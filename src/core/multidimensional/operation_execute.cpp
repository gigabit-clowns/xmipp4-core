// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/execution_context.hpp>
#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/kernel.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/multidimensional/kernel_manager.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include <core/logger.hpp>
#include <core/config.hpp>

#include <algorithm>
#include <sstream>

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
	boost::container::small_vector<array_descriptor, N> result;
	result.reserve(operands.size());
	for (const auto &a : operands)
	{
		result.push_back(a.get_descriptor());
	}
	return result;
}

template <std::size_t N>
boost::container::small_vector<numerical_type, N>
extract_data_types(
	span<const array_descriptor> descriptors,
	std::integral_constant<std::size_t, N> /*small_cap_tag*/
)
{
	boost::container::small_vector<numerical_type, N> result;
	result.reserve(descriptors.size());
	for (const auto &d : descriptors)
	{
		result.push_back(d.get_data_type());
	}
	return result;
}

template <std::size_t N>
boost::container::small_vector<operation_shape_policy::shape_type, N>
extract_shapes(
	span<const array_descriptor> descriptors,
	std::integral_constant<std::size_t, N> /*small_cap_tag*/
)
{
	boost::container::small_vector<operation_shape_policy::shape_type, N> result;
	result.reserve(descriptors.size());
	for (const auto &d : descriptors)
	{
		operation_shape_policy::shape_type shape;
		d.get_layout().get_extents(shape);
		result.push_back(std::move(shape));
	}
	return result;
}

void update_descriptors(
	span<array_descriptor> descriptors,
	span<const operation_shape_policy::shape_type> shapes,
	span<const numerical_type> data_types
)
{
	const auto n = descriptors.size();
	XMIPP4_ASSERT(n == shapes.size());
	XMIPP4_ASSERT(n == data_types.size());

	for (std::size_t i = 0; i < n; ++i)
	{
		descriptors[i] = array_descriptor(
			strided_layout::make_contiguous_layout(make_span(shapes[i])),
			data_types[i]
		);
	}
}

std::shared_ptr<hardware::buffer>
allocate_output_operand_storage(
	array &operand,
	const array_descriptor &descriptor,
	hardware::memory_allocator &allocator,
	std::size_t base_alignment,
	hardware::device_queue *queue
)
{
	const auto size = compute_storage_requirement(descriptor);
	const auto alignment = std::min(
		base_alignment,
		binary::bit_ceil(size)
	);
	auto storage = allocator.allocate(size, alignment, queue);
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
	const execution_context &context
)
{
	using result_type = boost::container::small_vector<
		std::shared_ptr<hardware::buffer>,
		N
	>;

	const auto n = operands.size();
	XMIPP4_ASSERT(n == descriptors.size());

	result_type result(operands.size());

	auto &allocator = context.get_memory_allocator(
		hardware::memory_resource_affinity::device
	);
	auto *queue = context.get_active_queue().get();
	const auto &properties = context.get_device_properties();
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

template <typename Ptr>
void record_queues(
	span<Ptr> storages,
	hardware::device_queue &queue
)
{
	for (const auto &storage : storages)
	{
		XMIPP4_ASSERT(storage);
		storage->record_queue(queue);
	}
}

void validate_arity(
	const operation &operation,
	std::size_t n_outputs,
	std::size_t n_inputs
)
{
	const auto expected_outputs = operation.get_output_count();
	const auto expected_inputs = operation.get_input_count();

	if (n_outputs != expected_outputs)
	{
		std::ostringstream oss;
		oss << "Operation '" << operation.get_name()
			<< "' expects " << expected_outputs
			<< " output(s), but " << n_outputs << " provided.";
		throw std::invalid_argument(oss.str());
	}

	if (n_inputs != expected_inputs)
	{
		std::ostringstream oss;
		oss << "Operation '" << operation.get_name()
			<< "' expects " << expected_inputs
			<< " input(s), but " << n_inputs << " provided.";
		throw std::invalid_argument(oss.str());
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
	using small_output_size_tag =
		std::integral_constant<std::size_t, XMIPP4_SMALL_OUTPUT_OPERAND_COUNT>;
	using small_input_size_tag =
		std::integral_constant<std::size_t, XMIPP4_SMALL_INPUT_OPERAND_COUNT>;

	const auto n_outputs = output_operands.size();
	const auto n_inputs = input_operands.size();

	validate_arity(operation, n_outputs, n_inputs);

	auto output_descriptors = extract_descriptors(
		output_operands, 
		small_output_size_tag()
	);
	auto output_shapes = extract_shapes(
		make_span(output_descriptors.data(), n_outputs),
		small_output_size_tag()
	);
	auto output_data_types = extract_data_types(
		make_span(output_descriptors.data(), n_outputs),
		small_output_size_tag()
	);

	auto input_descriptors = extract_descriptors(
		input_operands, 
		small_input_size_tag()
	);
	auto input_shapes = extract_shapes(
		make_span(input_descriptors.data(), n_inputs),
		small_input_size_tag()
	);
	auto input_data_types = extract_data_types(
		make_span(input_descriptors.data(), n_inputs),
		small_input_size_tag()
	);

	const auto outputs_initialized = std::all_of(
		output_operands.begin(),
		output_operands.end(),
		[](const array &a)
		{
			return static_cast<bool>(a.get_storage());
		}
	);

	const auto &shape_policy = operation.get_operation_shape_policy();
	const auto &data_type_policy = operation.get_operation_data_type_policy();

	if (!outputs_initialized)
	{
		shape_policy.deduce_output(
			make_span(output_shapes.data(), n_outputs),
			make_span(input_shapes.data(), n_inputs)
		);
		data_type_policy.deduce_output(
			make_span(output_data_types.data(), n_outputs),
			make_span(input_data_types.data(), n_inputs)
		);

		update_descriptors(
			make_span(output_descriptors.data(), n_outputs),
			make_span(output_shapes.data(), n_outputs),
			make_span(output_data_types.data(), n_outputs)
		);
	}

	shape_policy.validate(
		make_span(output_shapes.data(), n_outputs),
		make_span(input_shapes.data(), n_inputs)
	);
	data_type_policy.validate(
		make_span(output_data_types.data(), n_outputs),
		make_span(input_data_types.data(), n_inputs)
	);

	auto output_storages = resolve_output_storage(
		output_operands,
		make_span(output_descriptors.data(), n_outputs),
		small_output_size_tag(),
		context
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

	const auto &kernel_manager = context.get_kernel_manager();
	auto kernel = kernel_manager.build_kernel(
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

array execute(
	const operation &operation,
	span<const array_view> input_operands,
	const execution_context &context,
	array *out
)
{
	if (out)
	{
		execute(
			operation,
			make_span(out, 1),
			input_operands,
			context
		);

		return out->share();
	}
	else
	{
		array output_operand;
		execute(
			operation,
			make_span(&output_operand, 1),
			input_operands,
			context
		);
		return output_operand;
	}
}

array execute_unary(
	const operation &operation,
	array_view input,
	const execution_context &context,
	array *out
)
{
	return execute(
		operation,
		make_span(&input, 1),
		context,
		out
	);
}

array execute_binary(
	const operation &operation,
	array_view first_input,
	array_view second_input,
	const execution_context &context,
	array *out
)
{
	std::array<array_view, 2> inputs = {
		std::move(first_input),
		std::move(second_input)
	};
	return execute(
		operation,
		make_span(inputs),
		context,
		out
	);
}

array execute_ternary(
	const operation &operation,
	array_view first_input,
	array_view second_input,
	array_view third_input,
	const execution_context &context,
	array *out
)
{
	std::array<array_view, 3> inputs = {
		std::move(first_input),
		std::move(second_input),
		std::move(third_input)
	};
	return execute(
		operation,
		make_span(inputs),
		context,
		out
	);
}

} // namespace multidimensional
} // namespace xmipp4
