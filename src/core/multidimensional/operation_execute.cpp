// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/array_view.hpp>
#include <xmipp4/core/multidimensional/array_signature.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operation_data_type_policy.hpp>
#include <xmipp4/core/multidimensional/execution_context.hpp>
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

void execute(
	const operation &operation,
	span<array> output_operands,
	span<const array_view> input_operands,
	const execution_context &context
)
{
	const auto &dispatcher = context.get_dispatcher();
	if (dispatcher == nullptr)
	{
		throw std::invalid_argument(
			"execute: expected context with dereferenceable dispatcher."
		);
	}

	const auto &queue = context.get_active_queue();
	if (queue == nullptr)
	{
		throw std::invalid_argument(
			"execute: expected context with dereferenceable active queue."
		);
	}

	const auto &device_context = context.get_device_context();
	if (device_context == nullptr)
	{
		throw std::invalid_argument(
			"execute: expected context with dereferenceable device context."
		);
	}

	dispatcher->dispatch(
		operation,
		output_operands,
		input_operands,
		*device_context,
		*queue
	);
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
	const array_view &input,
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
