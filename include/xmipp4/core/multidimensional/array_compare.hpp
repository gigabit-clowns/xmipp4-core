// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"

namespace xmipp4 
{

class execution_context;

namespace multidimensional
{

/**
 * @brief Evaluate a < b for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array less(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate a <= b for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array less_equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate a > b for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array greater(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate a >= b for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array greater_equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate a == b for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array equal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate a != b for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array inequal(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
