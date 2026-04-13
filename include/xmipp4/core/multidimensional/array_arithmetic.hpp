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
 * @brief Evaluate `-x` for each element in the arrays.
 *
 * @param x Array to be negated. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting empty array.
 */
XMIPP4_CORE_API
array negate(
	const array_view &x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x + y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting addition.
 */
XMIPP4_CORE_API
array add(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x - y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting subtraction.
 */
XMIPP4_CORE_API
array subtract(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x * y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result of the multiplication.
 */
XMIPP4_CORE_API
array multiply(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x / y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The result of the division.
 */
XMIPP4_CORE_API
array divide(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
