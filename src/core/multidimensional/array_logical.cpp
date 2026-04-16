// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_logical.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/logical/logical_negate_operation.hpp>
#include <xmipp4/core/multidimensional/operations/logical/logical_and_operation.hpp>
#include <xmipp4/core/multidimensional/operations/logical/logical_or_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array logical_negate(array_view x, const execution_context &context, array *out)
{
	return execute_unary(
		logical_negate_operation(), 
		std::move(x), 
		context, 
		out
	);
}

array logical_and(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		logical_and_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

array logical_or(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		logical_or_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

} // namespace multidimensional
} // namespace xmipp4
