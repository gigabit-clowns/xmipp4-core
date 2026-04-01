// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_descriptor.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>

#include "../config.hpp"

#include <boost/container/small_vector.hpp>

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{
namespace
{

void populate_output_descriptors(
	const array* operands,
	array_descriptor* descriptors,
	std::size_t count
)
{	std::transform(
		operands, 
		operands + count,
		descriptors,
		std::mem_fn(&array::get_descriptor)
	);
}

void populate_input_descriptors(
	const array_view* operands,
	array_descriptor* descriptors,
	std::size_t count
)
{	std::transform(
		operands, 
		operands + count,
		descriptors,
		std::mem_fn(&array_view::get_descriptor)
	);
}

void populate_output_storages(
	array *operands,
	hardware::memory_resource_affinity* affinities,
	std::shared_ptr<hardware::buffer>* storages,
	std::size_t count
)
{
	// TODO
}

void populate_input_storages(
	const array_view *operands,
	hardware::memory_resource_affinity* affinities,
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
		// TODO check placement
		/*if (!check_storage_placement(*storages[i], affinities[i]))
		{
			throw std::invalid_argument(
				"One of the input operands is not placed in a suitable memory "
				"resource"
			);
		}*/
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
	boost::container::small_vector<
		array_descriptor, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT
	> input_descriptors(n_inputs);
	boost::container::small_vector<
		hardware::memory_resource_affinity, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT
	> output_affinities(n_outputs);
	boost::container::small_vector<
		hardware::memory_resource_affinity, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT
	> input_affinities(n_inputs);
	boost::container::small_vector<
		std::shared_ptr<hardware::buffer>, 
		XMIPP4_SMALL_OUTPUT_OPERAND_COUNT 
	> output_storages(n_outputs);
	boost::container::small_vector<
		std::shared_ptr<const hardware::buffer>, 
		XMIPP4_SMALL_INPUT_OPERAND_COUNT 
	> input_storages(n_inputs);

	populate_output_descriptors(
		output_operands.data(), 
		output_descriptors.data(), 
		n_outputs
	);
	populate_input_descriptors(
		input_operands.data(), 
		input_descriptors.data(), 
		n_inputs
	);

	operation.sanitize_operands(
		make_span(output_descriptors.data(), n_outputs),
		make_span(input_descriptors.data(), n_inputs)
	);
	operation.get_operand_affinities(
		make_span(output_affinities.data(), n_outputs),
		make_span(input_affinities.data(), n_inputs)
	);

	populate_output_storages(
		output_operands.data(), 
		output_affinities.data(),
		output_storages.data(),
		n_outputs
	);
	populate_input_storages(
		input_operands.data(), 
		input_affinities.data(),
		input_storages.data(),
		n_inputs
	);

	const auto& queue = context.get_active_queue();
	/*kernel.execute( // TODO
		make_span(output_storages.data(), n_outputs), 
		make_span(input_storages.data(), n_inputs), 
		queue.get()
	);*/


	if (queue)
	{
		record_queues(make_span(output_storages.data(), n_outputs), *queue);
		record_queues(make_span(input_storages.data(), n_inputs), *queue);
	}


	// TODO
	//       ___
    //      /   \
    //     |--o--|
    //     |  _  |
    //     |     |
    //     |     |
    //     |     |        C'mon,
    //     |     |\       do something...
    //     |     | \
    //     |     |  \
    //     |_____|   \
    //     |  |  |    \
    //     |  |  |     \
    //     |  |  |      \
    //     |  |  |       `--
    //     |  |  |
    //     |__|__|
}

} // namespace multidimensional
} // namespace xmipp4

