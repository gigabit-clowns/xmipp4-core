// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_trigonometric.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/array.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric/sin_operation.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric/cos_operation.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric/sincos_operation.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric/tan_operation.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric/asin_operation.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric/acos_operation.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric/atan_operation.hpp>
#include <xmipp4/core/multidimensional/operations/trigonometric/atan2_operation.hpp>

#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

array sin(array_view x, const execution_context &context, array *out)
{
	return execute_unary(sin_operation(), std::move(x), context, out);
}

array cos(array_view x, const execution_context &context, array *out)
{
	return execute_unary(cos_operation(), std::move(x), context, out);
}

void sincos(
	array_view x,
	const execution_context &context,
	array &sin,
	array &cos
)
{
	std::array<array, 2> output = { sin.share(), cos.share() };
	std::array<array_view, 1> input = { std::move(x) };

	execute(
		sincos_operation(),
		make_span(output),
		make_span(input),
		context
	);

	sin = std::move(output[0]);
	cos = std::move(output[1]);
}

array tan(array_view x, const execution_context &context, array *out)
{
	return execute_unary(tan_operation(), std::move(x), context, out);
}

array asin(array_view x, const execution_context &context, array *out)
{
	return execute_unary(asin_operation(), std::move(x), context, out);
}

array acos(array_view x, const execution_context &context, array *out)
{
	return execute_unary(acos_operation(), std::move(x), context, out);
}

array atan(array_view x, const execution_context &context, array *out)
{
	return execute_unary(atan_operation(), std::move(x), context, out);
}

array atan2(
	array_view y,
	array_view x,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		atan2_operation(), 
		std::move(y), 
		std::move(x), 
		context, 
		out
	);
}

} // namespace multidimensional
} // namespace xmipp4
