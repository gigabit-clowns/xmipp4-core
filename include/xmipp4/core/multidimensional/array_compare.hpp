// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"

namespace xmipp4 
{

class execution_context;

namespace multidimensional
{

class array_view;

/**
 * @brief Evaluate `x < y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting comparison.
 */
XMIPP4_CORE_API
array less(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x <= y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting comparison.
 */
XMIPP4_CORE_API
array less_equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x > y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting comparison.
 */
XMIPP4_CORE_API
array greater(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x >= y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting comparison.
 */
XMIPP4_CORE_API
array greater_equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x == y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting comparison.
 */
XMIPP4_CORE_API
array equal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x != y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting comparison.
 */
XMIPP4_CORE_API
array inequal(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
