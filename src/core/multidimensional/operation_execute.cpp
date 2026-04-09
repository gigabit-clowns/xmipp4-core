// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/logger.hpp>
#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/multidimensional/array_factory.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include "../config.hpp"

#include <boost/container/small_vector.hpp>

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{
namespace
{

template <typename Arr>
void populate_array_descriptors(
	const Arr* operands,
	array_descriptor* descriptors,
	std::size_t count
)
{	std::transform(
		operands, 
		operands + count,
		descriptors,
		[] (const auto &a)
		{
			return a.get_descriptor();
		}
	);
}

void populate_output_storages(
	array *operands,
	const array_descriptor *descriptors,
	std::shared_ptr<hardware::buffer>* storages,
	std::size_t count,
	const execution_context &context
)
{
	auto &allocator = context.get_memory_allocator(
		hardware::memory_resource_affinity::device
	);
	auto *queue = context.get_active_queue().get();
	const auto &properties = context.get_device_properties();
    const auto max_alignment = allocator.get_max_alignment();
    const auto preferred_alignment = properties.get_optimal_data_alignment();

	for (std::size_t i = 0; i < count; ++i)
	{
		auto storage = operands[i].share_storage();
		auto size = compute_storage_requirement(descriptors[i]);
		if (storage)
		{
			if (storage->get_size() < size)
			{
				throw std::invalid_argument(
					"An output array without sufficient storage was provided"
				);
			}

			if (operands[i].get_descriptor() != descriptors[i])
			{
				XMIPP4_LOG_WARN(
					"Provided output operand's descriptor is overriden due "
					"to a mismatch."
				);
				operands[i] = array(storage, descriptors[i]);
			}
		}
		else
		{
			size = binary::bit_ceil(size);
			const auto alignment = 
				std::min(std::min(max_alignment, preferred_alignment), size);
			storage = allocator.allocate(size, alignment, queue);
			operands[i] = array(storage, descriptors[i]);
		}

		storages[i] = std::move(storage);
	}
}

void populate_input_storages(
	const array_view *operands,
	std::shared_ptr<const hardware::buffer>* storages,
	std::size_t count
)
{
	for (std::size_t i = 0; i < count; ++i)
	{
		storages[i] = operands[i].share_storage();
		if (!storages[i])
		{
			throw std::invalid_argument(
				"One of the input operands does not an associated storage"
			);
		}
	}
}

template <typename Ptr>
void populate_array_signatures(
	const array_descriptor *descriptors,
	const Ptr* storages,
	array_signature *signatures,
	std::size_t count
)
{
	for (std::size_t i = 0; i < count; ++i)
	{
		XMIPP4_ASSERT(storages[i]);
		signatures[i] = array_signature(
			descriptors[i],
			&(storages[i]->get_memory_resource())
		);
	}
}

void record_queues(
	span<const std::shared_ptr<hardware::buffer>> storages,
	hardware::device_queue &queue
)
{		
	for (const auto &storage : storages)
	{
		XMIPP4_ASSERT(storage);
		storage->record_queue(queue);
	}
}

void record_queues(
	span<const std::shared_ptr<const hardware::buffer>> storages,
	hardware::device_queue &queue
)
{		
	for (const auto &storage : storages)
	{
		XMIPP4_ASSERT(storage);
		storage->record_queue(queue);
	}
}

} // anonymous namespace



void execute(
	const operation &operation,
	span<array> output_operands,
	span<const array_view> input_operands,
	const execution_context &context
)
{
	const auto n_outputs = output_operands.size();
	const auto n_inputs = input_operands.size();

	boost::container::small_vector<
		array_descriptor, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT
	> output_descriptors(n_outputs);
	populate_array_descriptors(
		output_operands.data(), 
		output_descriptors.data(), 
		n_outputs
	);

	boost::container::small_vector<
		array_descriptor, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT
	> input_descriptors(n_inputs);
	populate_array_descriptors(
		input_operands.data(), 
		input_descriptors.data(), 
		n_inputs
	);

	operation.sanitize_operands(
		make_span(output_descriptors.data(), n_outputs),
		make_span(input_descriptors.data(), n_inputs)
	);

	boost::container::small_vector<
		std::shared_ptr<hardware::buffer>, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT 
	> output_storages(n_outputs);
	populate_output_storages(
		output_operands.data(), 
		output_descriptors.data(),
		output_storages.data(),
		n_outputs,
		context
	);

	boost::container::small_vector<
		std::shared_ptr<const hardware::buffer>, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT 
	> input_storages(n_inputs);
	populate_input_storages(
		input_operands.data(), 
		input_storages.data(),
		n_inputs
	);

	boost::container::small_vector<
		array_signature, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT
	> output_signatures(n_outputs);
	populate_array_signatures(
		output_descriptors.data(),
		output_storages.data(),
		output_signatures.data(),
		n_outputs
	);

	boost::container::small_vector<
		array_signature, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT
	> input_signatures(n_inputs);
	populate_array_signatures(
		input_descriptors.data(),
		input_storages.data(),
		input_signatures.data(),
		n_inputs
	);

	kernel_manager manager; // TODO obtain
	auto kernel = manager.build_kernel(
		operation,
		xmipp4::make_span(output_signatures.data(), n_outputs),
		xmipp4::make_span(input_signatures.data(), n_inputs)
	);

	const auto& queue = context.get_active_queue();

	kernel->execute(
		make_span(output_storages.data(), n_outputs), 
		make_span(input_storages.data(), n_inputs), 
		queue.get()
	);

	if (queue)
	{
		record_queues(make_span(output_storages.data(), n_outputs), *queue);
		record_queues(make_span(input_storages.data(), n_inputs), *queue);
	}
}

} // namespace multidimensional
} // namespace xmipp4
