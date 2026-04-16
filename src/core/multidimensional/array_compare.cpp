// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_compare.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/compare/less_operation.hpp>
#include <xmipp4/core/multidimensional/operations/compare/less_equal_operation.hpp>
#include <xmipp4/core/multidimensional/operations/compare/equal_operation.hpp>
#include <xmipp4/core/multidimensional/operations/compare/inequal_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array less(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		less_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

array less_equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		less_equal_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

array greater(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	// using less and swapping operands.
	return execute_binary(
		less_operation(), 
		std::move(rhs), 
		std::move(lhs), 
		context, 
		out
	);
}

array greater_equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	// using less_equal and swapping operands.
	return execute_binary(
		less_equal_operation(), 
		std::move(rhs), 
		std::move(lhs), 
		context, 
		out
	);
}

array equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		equal_operation(), 
		std::move(rhs), 
		std::move(lhs), 
		context, 
		out
	);
}

array inequal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		inequal_operation(), 
		std::move(rhs), 
		std::move(lhs), 
		context, 
		out
	);
}

} // namespace multidimensional
} // namespace xmipp4
