// SPDX-License-Identifier: GPL-3.0-only

#include "eager_operation_dispatcher.hpp"

#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_program_manager.hpp>
#include <xmipp4/core/hardware/program.hpp>
#include <xmipp4/core/hardware/command.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>
#include <xmipp4/core/hardware/program_scratch_requirement.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/device_instance.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/buffer.hpp>

#include <core/logger.hpp>
#include <core/config.hpp>
#include <core/hardware/memory_allocator_table.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <vector>

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

template <std::size_t N>
boost::container::small_vector<operation_shape_policy::shape_type, N>
make_empty_shapes(
	std::size_t count,
	std::integral_constant<std::size_t, N> /*small_cap_tag*/
)
{
	return boost::container::small_vector<
		operation_shape_policy::shape_type, N
	>(count);
}

template <std::size_t N>
boost::container::small_vector<numerical_type, N>
make_empty_data_types(
	std::size_t count,
	std::integral_constant<std::size_t, N> /*small_cap_tag*/
)
{
	return boost::container::small_vector<numerical_type, N>(count);
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
	const hardware::device_context &device_context,
	hardware::command_queue &queue,
	std::integral_constant<std::size_t, N> /*small_cap_tag*/
)
{
	using result_type = boost::container::small_vector<
		std::shared_ptr<hardware::buffer>,
		N
	>;

	const auto n = operands.size();
	XMIPP4_ASSERT(n == descriptors.size());

	result_type result(operands.size());

	const auto &allocator = device_context.get_allocator(
		hardware::memory_resource_affinity::device
	);
	const auto &properties = 
		device_context.get_device_instance()->get_properties();
	const auto max_alignment = allocator->get_max_alignment();
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
				*allocator,
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

void validate_arity(
	const operation_arity& user,
	const operation_arity& expected
)
{
	if (user.get_output_count() != expected.get_output_count())
	{
		std::ostringstream oss;
		oss << "Expected " << expected.get_output_count()
			<< " output(s), but " << user.get_output_count() << " provided.";
		throw std::invalid_argument(oss.str());
	}

	if (user.get_input_count() != expected.get_input_count())
	{
		std::ostringstream oss;
		oss << "Expected " << expected.get_input_count()
			<< " input(s), but " << user.get_input_count() << " provided.";
		throw std::invalid_argument(oss.str());
	}
}

template <std::size_t N>
boost::container::small_vector<std::shared_ptr<hardware::buffer>, N>
allocate_scratch(
	span<const hardware::program_scratch_requirement> requirements,
	const hardware::device_context &device_context,
	hardware::command_queue &queue,
	std::integral_constant<std::size_t, N> /*small_cap_tag*/
)
{
	using result_type = boost::container::small_vector<
		std::shared_ptr<hardware::buffer>,
		N
	>;

	result_type result(requirements.size());

	for (std::size_t i = 0; i < requirements.size(); ++i)
	{
		const auto &requirement = requirements[i];
		const auto size = requirement.get_size();
		const auto alignment = requirement.get_alignment();
		const auto affinity = requirement.get_affinity();
		const auto &allocator = device_context.get_allocator(affinity);
		result[i] = allocator->allocate(size, alignment, &queue);
	}

	return result;
}
} // anonymous namespace

eager_operation_dispatcher::eager_operation_dispatcher(
	std::shared_ptr<const operation_program_manager> program_manager,
	std::size_t cache_capacity
)
	: m_program_manager(std::move(program_manager))
	, m_program_cache(cache_capacity)
{
	if (!m_program_manager)
	{
		throw std::invalid_argument(
			"program_manager cannot be null"
		);
	}
}

eager_operation_dispatcher::~eager_operation_dispatcher() = default;

void eager_operation_dispatcher::dispatch(
	const operation &op,
	span<array> output_operands,
	span<const array_view> input_operands,
	const hardware::device_context &device_context
)
{
	using small_output_size_tag =
		std::integral_constant<std::size_t, XMIPP4_SMALL_OUTPUT_OPERAND_COUNT>;
	using small_input_size_tag =
		std::integral_constant<std::size_t, XMIPP4_SMALL_INPUT_OPERAND_COUNT>;
	using small_scratch_size_tag =
		std::integral_constant<std::size_t, XMIPP4_SMALL_SCRATCH_OPERAND_COUNT>;

	const auto &queue = device_context.get_active_queue();
	if (!queue)
	{
		throw std::invalid_argument(
			"Expected a device context with a dereferenceable active queue."
		);
	}
	
	const auto n_outputs = output_operands.size();
	const auto n_inputs = input_operands.size();

	validate_arity(operation_arity(n_outputs, n_inputs), op.get_arity());

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

	// Outputs are treated as user-provided only when every one of them
	// already owns storage. A partially populated set is handled as if no
	// outputs were supplied (see the else branch below).
	const auto outputs_initialized = std::all_of(
		output_operands.begin(),
		output_operands.end(),
		[](const array &a)
		{
			return static_cast<bool>(a.get_storage());
		}
	);

	const auto &shape_policy = op.get_operation_shape_policy();
	const auto &data_type_policy = op.get_operation_data_type_policy();

	// Deduce canonical output shapes/types from the inputs alone. This
	// performs all input-side validation as a side effect.
	auto output_shapes =
		make_empty_shapes(n_outputs, small_output_size_tag());
	auto output_data_types =
		make_empty_data_types(n_outputs, small_output_size_tag());

	shape_policy.deduce(
		make_span(output_shapes.data(), n_outputs),
		make_span(input_shapes.data(), n_inputs)
	);
	data_type_policy.deduce(
		make_span(output_data_types.data(), n_outputs),
		make_span(input_data_types.data(), n_inputs)
	);

	auto output_descriptors = extract_descriptors(
		output_operands,
		small_output_size_tag()
	);

	if (outputs_initialized)
	{
		// Outputs come from the user: confirm they are admissible given
		// the canonical ones produced above. The user-supplied descriptors
		// (already held in output_descriptors) are kept as-is going
		// forward; they may legitimately differ from the canonical ones
		// (e.g. a wider broadcast-compatible shape, or a converted type).
		auto user_output_shapes = extract_shapes(
			make_span(output_descriptors.data(), n_outputs),
			small_output_size_tag()
		);
		auto user_output_data_types = extract_data_types(
			make_span(output_descriptors.data(), n_outputs),
			small_output_size_tag()
		);

		shape_policy.accept(
			make_span(user_output_shapes.data(), n_outputs),
			make_span(output_shapes.data(), n_outputs),
			make_span(input_shapes.data(), n_inputs)
		);
		data_type_policy.accept(
			make_span(user_output_data_types.data(), n_outputs),
			make_span(output_data_types.data(), n_outputs),
			make_span(input_data_types.data(), n_inputs)
		);
	}
	else
	{
		// Outputs were not pre-allocated: install the canonical ones.
		update_descriptors(
			make_span(output_descriptors.data(), n_outputs),
			make_span(output_shapes.data(), n_outputs),
			make_span(output_data_types.data(), n_outputs)
		);
	}

	auto output_storages = resolve_output_storage(
		output_operands,
		make_span(output_descriptors.data(), n_outputs),
		device_context,
		*queue,
		small_output_size_tag()
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

	XMIPP4_ASSERT(m_program_manager);
	const auto prog = m_program_manager->build(
		op,
		xmipp4::make_span(output_signatures.data(), n_outputs),
		xmipp4::make_span(input_signatures.data(), n_inputs),
		*queue,
		&m_program_cache
	);
	XMIPP4_ASSERT(prog);

	auto scratch = allocate_scratch(
		prog->get_scratch_requirements(),
		device_context,
		*queue,
		small_scratch_size_tag()
	);

	const auto cmd = hardware::command(std::move(prog))
		.bind_outputs(xmipp4::make_span(output_storages.data(), n_outputs))
		.bind_inputs(xmipp4::make_span(input_storages.data(), n_inputs))
		.bind_scratch(xmipp4::make_span(scratch.data(), scratch.size()));

	queue->submit(cmd);
}

// Declared in operation_dispatcher.hpp.
std::shared_ptr<operation_dispatcher> make_eager_operation_dispatcher(
	std::shared_ptr<const operation_program_manager> program_manager
)
{
	return std::make_shared<eager_operation_dispatcher>(
		std::move(program_manager),
		XMIPP4_DEFAULT_OPERATION_PROGRAM_CACHE_CAPACITY
	);
}

} // namespace multidimensional
} // namespace xmipp4
