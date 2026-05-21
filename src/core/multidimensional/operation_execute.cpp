// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/hardware/command.hpp>
#include <xmipp4/core/multidimensional/operation_command_manager.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>
#include <xmipp4/core/hardware/command.hpp>

#include <core/logger.hpp>
#include <core/config.hpp>

#include <algorithm>

#include <boost/container/small_vector.hpp>

namespace xmipp4 
{
namespace multidimensional
{
namespace
{

template <typename ArrayType, std::size_t N>
boost::container::small_vector<array_descriptor, N>
extract_descriptors(
	span<ArrayType> operands, 
	std::integral_constant<std::size_t, N> /*small_cap_tag*/
)
{
    boost::container::small_vector<array_descriptor, N> result(operands.size());

    std::transform(
        operands.begin(), 
        operands.end(),
        result.begin(),
        [](const auto &a) { return a.get_descriptor(); }
    );

    return result;
}

std::shared_ptr<hardware::buffer>
allocate_output_operand_storage(
	array &operand,
	const array_descriptor &descriptor,
	hardware::memory_allocator &allocator,
	std::size_t base_alignment,
	hardware::command_queue &queue
)
{
	const auto size = compute_storage_requirement(descriptor);
	const auto alignment = std::min(
		base_alignment,
		binary::bit_ceil(size)
	);
	auto storage = allocator.allocate(size, alignment, &queue);
	operand = array(storage, descriptor);
	return storage;
}

void validate_output_operand_storage(
	array &operand,
	const array_descriptor &descriptor,
	const std::shared_ptr<hardware::buffer> &storage
)
{
	const auto size = compute_storage_requirement(descriptor);

	if (storage->get_size() < size)
	{
		throw std::invalid_argument(
			"An output array without sufficient storage was provided"
		);
	}

	if (operand.get_descriptor() != descriptor)
	{
		XMIPP4_LOG_WARN(
			"Provided output operand's descriptor is overriden due "
			"to a mismatch."
		);
		operand = array(storage, descriptor);
	}
}

template <std::size_t N>
boost::container::small_vector<std::shared_ptr<hardware::buffer>, N>
resolve_output_storage(
	span<array> operands,
	span<const array_descriptor> descriptors,
	std::integral_constant<std::size_t, N> /*small_cap_tag*/,
	hardware::memory_allocator &allocator,
	hardware::command_queue &queue,
	const hardware::device_properties &properties
)
{
	using result_type = boost::container::small_vector<
		std::shared_ptr<hardware::buffer>, 
		N
	>; 

	const auto n = operands.size();
	XMIPP4_ASSERT(n == descriptors.size());

	result_type result(operands.size());

    const auto max_alignment = allocator.get_max_alignment();
    const auto preferred_alignment = properties.get_optimal_data_alignment();
	const auto base_alignment = std::min(max_alignment, preferred_alignment);

    for (std::size_t i = 0; i < n; ++i)
    {
		auto storage = operands[i].share_storage();
		if (storage)
		{
			validate_output_operand_storage(
				operands[i], 
				descriptors[i], 
				storage
			);
		}
		else
		{
			storage = allocate_output_operand_storage(
				operands[i],
				descriptors[i],
				allocator,
				base_alignment,
				queue
			);
		}	

		XMIPP4_ASSERT(storage);
		result[i] = std::move(storage);
    }

    return result;
}

template <std::size_t N>
boost::container::small_vector<std::shared_ptr<const hardware::buffer>, N>
resolve_input_storage(
	span<const array_view> operands,
	std::integral_constant<std::size_t, N> /*small_cap_tag*/
)
{
	using result_type = boost::container::small_vector<
		std::shared_ptr<const hardware::buffer>, 
		N
	>; 
	result_type result(operands.size());

    for (std::size_t i = 0; i < operands.size(); ++i)
    {
        result[i] = operands[i].share_storage();
        if (!result[i])
        {
            throw std::invalid_argument(
				"One of the input operands does not have associated storage. "
				"Input arrays must be populated before calling execute."
			);
        }
    }

    return result;
}

template <typename Ptr, std::size_t N>
boost::container::small_vector<array_signature, N>
create_signatures(
	boost::container::small_vector<array_descriptor, N> &&descriptors,
	span<Ptr> storages
)
{
	const auto n = descriptors.size();
	XMIPP4_ASSERT(n == storages.size());

	boost::container::small_vector<array_signature, N> result(n);
	for (std::size_t i = 0; i < n; ++i)
	{
		XMIPP4_ASSERT(storages[i]);
        result[i] = array_signature(
            std::move(descriptors[i]), // Steal descriptors
            &(storages[i]->get_memory_resource())
        );
	}

	return result;
}

std::shared_ptr<hardware::buffer> allocate_scratch(
	const hardware::command &cmd,
	hardware::memory_allocator &allocator,
	hardware::command_queue &queue
)
{
	std::shared_ptr<hardware::buffer> result;

	std::size_t size;
	std::size_t alignment;
	if (cmd.get_scratch_requirement(size, alignment))
	{
		result = allocator.allocate(size, alignment, &queue);
	}

	return result;
}

} // anonymous namespace



void execute(
	const operation &operation,
	span<array> output_operands,
	span<const array_view> input_operands,
	const execution_context &context
)
{
	using small_output_size_tag = 
		std::integral_constant<std::size_t, XMIPP4_SMALL_OUTPUT_OPERAND_COUNT>;
	using small_input_size_tag = 
		std::integral_constant<std::size_t, XMIPP4_SMALL_INPUT_OPERAND_COUNT>;

	const auto n_outputs = output_operands.size();
	const auto n_inputs = input_operands.size();
	const auto& queue = context.get_active_queue();
	const auto& allocator = context.get_active_allocator();
	const auto& properties = context.get_device_properties();

	auto output_descriptors = 
		extract_descriptors(output_operands, small_output_size_tag());
	auto input_descriptors = 
		extract_descriptors(input_operands, small_input_size_tag());

	operation.sanitize_operands(
		make_span(output_descriptors.data(), n_outputs),
		make_span(input_descriptors.data(), n_inputs)
	);

	auto output_storages = resolve_output_storage(
		output_operands, 
		make_span(output_descriptors.data(), n_outputs),
		small_output_size_tag(),
		*allocator,
		*queue,
		properties
	);
	auto input_storages = resolve_input_storage(
		input_operands, 
		small_input_size_tag()
	);

	auto output_signatures = create_signatures(
		std::move(output_descriptors), // No longer needed.
		make_span(output_storages.data(), n_outputs)
	);
	auto input_signatures = create_signatures(
		std::move(input_descriptors),  // No longer needed.
		make_span(input_storages.data(), n_inputs)
	);

	operation_command_manager command_manager; // TODO obtain
	const auto command = command_manager.build(
		operation,
		xmipp4::make_span(output_signatures.data(), n_outputs),
		xmipp4::make_span(input_signatures.data(), n_inputs)
	);

	const auto scratch = allocate_scratch(*command, *allocator, *queue);
	queue->submit(
		*command,
		make_span(output_storages.data(), n_outputs),
		make_span(input_storages.data(), n_inputs),
		scratch
	);
}

} // namespace multidimensional
} // namespace xmipp4
