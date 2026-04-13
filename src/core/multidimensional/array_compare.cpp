// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_compare.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/compare_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array less(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(less_operation(), lhs, rhs, context, out);
}

array less_equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(less_equal_operation(), lhs, rhs, context, out);
}

array greater(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	// using less and swapping operands.
	return execute_binary(less_operation(), rhs, lhs, context, out);
}

array greater_equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	// using less_equal and swapping operands.
	return execute_binary(less_equal_operation(), rhs, lhs, context, out);
}

array equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(equal_operation(), lhs, rhs, context, out);
}

array inequal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(inequal_operation(), lhs, rhs, context, out);
}

} // namespace multidimensional
} // namespace xmipp4
