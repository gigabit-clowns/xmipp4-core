// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_bitwise.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/bitwise_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array bitwise_negate(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(bitwise_negate_operation(), x, context, out);
}

array bitwise_and(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(bitwise_and_operation(), lhs, rhs, context, out);
}

array bitwise_or(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(bitwise_or_operation(), lhs, rhs, context, out);
}

array bitwise_xor(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(bitwise_xor_operation(), lhs, rhs, context, out);
}

} // namespace multidimensional
} // namespace xmipp4
