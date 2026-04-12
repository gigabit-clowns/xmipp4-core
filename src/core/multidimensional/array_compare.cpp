// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_compare.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/compare_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

namespace
{

array execute_comparison(
	const compare_operation &comparison,
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	std::array<array_view, 2> input_operands = { lhs.share(), rhs.share() };
	return execute(
		comparison,
		make_span(input_operands),
		context,
		out
	);
}

} // anonymous namespace

array less(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_comparison(less_operation(), lhs, rhs, context, out);
}

array less_equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_comparison(less_equal_operation(), lhs, rhs, context, out);
}

array greater(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_comparison(greater_operation(), lhs, rhs, context, out);
}

array greater_equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_comparison(greater_equal_operation(), lhs, rhs, context, out);
}

array equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_comparison(equal_operation(), lhs, rhs, context, out);
}

array inequal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out
)
{
	return execute_comparison(inequal_operation(), lhs, rhs, context, out);
}

} // namespace multidimensional
} // namespace xmipp4
