// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_bitwise.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_negate_operation.hpp>
#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_and_operation.hpp>
#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_or_operation.hpp>
#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_xor_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array bitwise_negate(
	array_view x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(
		bitwise_negate_operation(), 
		std::move(x), 
		context, 
		out
	);
}

array bitwise_and(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		bitwise_and_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

array bitwise_or(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		bitwise_or_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

array bitwise_xor(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		bitwise_xor_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

} // namespace multidimensional
} // namespace xmipp4
