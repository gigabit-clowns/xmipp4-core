// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_power.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/power/power_operation.hpp>
#include <xmipp4/core/multidimensional/operations/power/exp_operation.hpp>
#include <xmipp4/core/multidimensional/operations/power/log_operation.hpp>
#include <xmipp4/core/multidimensional/operations/power/square_operation.hpp>
#include <xmipp4/core/multidimensional/operations/power/sqrt_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array pow(
	array_view base,
	array_view exponent,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		power_operation(), 
		std::move(base), 
		std::move(exponent), 
		context, 
		out
	);
}

array exp(array_view x, const execution_context &context, array *out)
{
	return execute_unary(exp_operation(), std::move(x), context, out);
}

array log(array_view x, const execution_context &context, array *out)
{
	return execute_unary(log_operation(), std::move(x), context, out);
}

array square(array_view x, const execution_context &context, array *out)
{
	return execute_unary(square_operation(), std::move(x), context, out);
}

array sqrt(array_view x, const execution_context &context, array *out)
{
	return execute_unary(sqrt_operation(), std::move(x), context, out);
}

} // namespace multidimensional
} // namespace xmipp4
