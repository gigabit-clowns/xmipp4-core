// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/functional/arithmetic.hpp>

#include <xmipp4/core/dispatch/execute.hpp>
#include <xmipp4/ops/arithmetic/add_operation.hpp>
#include <xmipp4/ops/arithmetic/divide_operation.hpp>
#include <xmipp4/ops/arithmetic/modulo_operation.hpp>
#include <xmipp4/ops/arithmetic/multiply_operation.hpp>
#include <xmipp4/ops/arithmetic/negate_operation.hpp>
#include <xmipp4/ops/arithmetic/subtract_operation.hpp>

namespace xmipp4
{

array add(
	const_array_ref& x,
	const_array_ref& y,
	const execution_context &context,
	array *out
)
{
	return execute_binary(ops::add_operation(), x, y, context, out);
}

array subtract(
	const_array_ref& x,
	const_array_ref& y,
	const execution_context &context,
	array *out
)
{
	return execute_binary(ops::subtract_operation(), x, y, context, out);
}

array negate(
	const_array_ref& x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(ops::negate_operation(), x, context, out);
}

array multiply(
	const_array_ref& x,
	const_array_ref& y,
	const execution_context &context,
	array *out
)
{
	return execute_binary(ops::multiply_operation(), x, y, context, out);
}

array divide(
	const_array_ref& x,
	const_array_ref& y,
	const execution_context &context,
	array *out
)
{
	return execute_binary(ops::divide_operation(), x, y, context, out);
}

array modulo(
	const_array_ref& x,
	const_array_ref& y,
	const execution_context &context,
	array *out
)
{
	return execute_binary(ops::modulo_operation(), x, y, context, out);
}

} // namespace xmipp4
