// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "array_view.hpp"

namespace xmipp4 
{

class execution_context;

namespace multidimensional
{

/**
 * @brief Evaluate `~x` for each element in the arrays.
 *
 * @param x Array to be negated. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting logical negation.
 */
XMIPP4_CORE_API
array logical_negate(
	array_view x,
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
 * @return array The resulting logical anding.
 */
XMIPP4_CORE_API
array logical_and(
	array_view lhs,
	array_view rhs,
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
 * @return array The resulting logical oring.
 */
XMIPP4_CORE_API
array logical_or(
	array_view lhs,
	array_view rhs,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `mask ? x : y` for each element in the arrays.
 *
 * The mask array must be boolean. The x and y arrays must share the same
 * numerical type, which becomes the type of the output.
 *
 * @param mask Boolean selector array.
 * @param x Values selected where mask is true.
 * @param y Values selected where mask is false.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting selected values.
 */
XMIPP4_CORE_API
array select(
	array_view mask,
	array_view x,
	array_view y,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
