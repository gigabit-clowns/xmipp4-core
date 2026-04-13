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
 * @brief Evaluate `~x` for each element in the arrays.
 *
 * @param x Array to be negated. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting bitwise negation.
 */
XMIPP4_CORE_API
array bitwise_negate(
	const array_view &x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x & y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting bitwise anding.
 */
XMIPP4_CORE_API
array bitwise_and(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x | y` for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting bitwise oring.
 */
XMIPP4_CORE_API
array bitwise_or(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `x ^ y` (exclusive or) for each element in the arrays.
 *
 * @param lhs Left hand side operand. 
 * @param rhs Right hand side operand. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting bitwise exclusive oring.
 */
XMIPP4_CORE_API
array bitwise_xor(
	const array_view &lhs,
	const array_view &rhs,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
