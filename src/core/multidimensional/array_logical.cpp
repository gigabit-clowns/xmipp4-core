// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_logical.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/logical_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array logical_negate(
	const array_view &x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(logical_negate_operation(), x, context, out);
}

array logical_and(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(logical_and_operation(), lhs, rhs, context, out);
}

array logical_or(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(logical_or_operation(), lhs, rhs, context, out);
}

} // namespace multidimensional
} // namespace xmipp4
