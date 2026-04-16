// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_arithmetic.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/arithmetic/negate_operation.hpp>
#include <xmipp4/core/multidimensional/operations/arithmetic/add_operation.hpp>
#include <xmipp4/core/multidimensional/operations/arithmetic/subtract_operation.hpp>
#include <xmipp4/core/multidimensional/operations/arithmetic/multiply_operation.hpp>
#include <xmipp4/core/multidimensional/operations/arithmetic/divide_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

array negate(
	array_view x,
	const execution_context &context,
	array *out
)
{
	return execute_unary(
		negate_operation(), 
		std::move(x), 
		context, 
		out
	);
}

array add(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		add_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

array subtract(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		subtract_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

array multiply(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		multiply_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

array divide(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out
)
{
	return execute_binary(
		divide_operation(), 
		std::move(lhs), 
		std::move(rhs), 
		context, 
		out
	);
}

} // namespace multidimensional
} // namespace xmipp4
