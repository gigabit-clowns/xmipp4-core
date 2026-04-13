// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_trigonometric.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric_operation.hpp>

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

array sin(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(sin_operation(), x, context, out); // TODO
}

array cos(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(cos_operation(), x, context, out); // TODO
}

void sincos(
	const array_view &x,
	const execution_context &context,
	array &sin,
	array &cos
)
{
	std::array<array, 2> output = { sin.share(), cos.share() };
	std::array<array_view, 1> input = { x.share() };

	execute(
		sincos_operation(),
		make_span(output),
		make_span(input),
		context
	);

	sin = std::move(output[0]);
	cos = std::move(output[1]);
}

array tan(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(tan_operation(), x, context, out); // TODO
}

array asin(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(asin_operation(), x, context, out); // TODO
}

array acos(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(acos_operation(), x, context, out); // TODO
}

array atan(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(atan_operation(), x, context, out); // TODO
}

array atan2(
	const array_view &y,
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_binary(atan2_operation(), y, x, context, out); // TODO
}

} // namespace multidimensional
} // namespace xmipp4
