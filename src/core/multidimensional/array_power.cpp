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
	const array_view &base,
	const array_view &exponent,
	const execution_context &context,
	array *out
)
{
	return execute_binary(power_operation(), base, exponent, context, out);
}

array exp(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(exp_operation(), x, context, out);
}

array log(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(log_operation(), x, context, out);
}

array square(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(square_operation(), x, context, out);
}

array sqrt(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(sqrt_operation(), x, context, out);
}

} // namespace multidimensional
} // namespace xmipp4
