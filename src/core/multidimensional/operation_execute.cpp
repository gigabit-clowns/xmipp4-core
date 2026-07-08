// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_execute.hpp>

#include <xmipp4/core/binary/bit.hpp>
#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/array_view.hpp>
#include <xmipp4/core/ndarray/array_signature.hpp>
#include <xmipp4/core/layout/strided_layout.hpp>
#include <xmipp4/core/operations/operation.hpp>
#include <xmipp4/core/operations/shape_policy.hpp>
#include <xmipp4/core/operations/data_type_policy.hpp>
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
	const operations::operation &operation,
	span<ndarray::array> output_operands,
	span<const ndarray::array_view> input_operands,
	const execution_context &context
)
{
	const auto &device_context = context.get_device_context();
	const auto &dispatcher = context.get_dispatcher();
	if (dispatcher == nullptr)
	{
		throw std::invalid_argument(
			"execute: expected context with dereferenceable dispatcher."
		);
	}

	dispatcher->dispatch(
		operation,
		output_operands,
		input_operands,
		device_context
	);
}

ndarray::array execute(
	const operations::operation &operation,
	span<const ndarray::array_view> input_operands,
	const execution_context &context,
	ndarray::array *out
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
		ndarray::array output_operand;
		execute(
			operation,
			make_span(&output_operand, 1),
			input_operands,
			context
		);
		return output_operand;
	}
}

ndarray::array execute_unary(
	const operations::operation &operation,
	const ndarray::array_view &input,
	const execution_context &context,
	ndarray::array *out
)
{
	return execute(
		operation,
		make_span(&input, 1),
		context,
		out
	);
}

ndarray::array execute_binary(
	const operations::operation &operation,
	ndarray::array_view first_input,
	ndarray::array_view second_input,
	const execution_context &context,
	ndarray::array *out
)
{
	std::array<ndarray::array_view, 2> inputs = {
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

ndarray::array execute_ternary(
	const operations::operation &operation,
	ndarray::array_view first_input,
	ndarray::array_view second_input,
	ndarray::array_view third_input,
	const execution_context &context,
	ndarray::array *out
)
{
	std::array<ndarray::array_view, 3> inputs = {
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
