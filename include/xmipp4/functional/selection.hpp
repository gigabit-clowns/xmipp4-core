// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/ndarray/array.hpp>
#include <xmipp4/core/ndarray/const_array_ref.hpp>

namespace xmipp4
{

class execution_context;

/**
 * @brief Select element-wise between two arrays according to a condition.
 *
 * @param condition The boolean array selecting between the other two.
 * @param x The array selected where the condition holds.
 * @param y The array selected where it does not.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The array holding the element-wise selection.
 *
 * @note The condition must be a boolean array. All three inputs must be
 * broadcast-compatible.
 */
XMIPP4_CORE_API
array where(
	const_array_ref condition,
	const_array_ref x,
	const_array_ref y,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Confine the elements of an array to an interval.
 *
 * @param x The array to be confined.
 * @param lower The lower bound.
 * @param upper The upper bound.
 * @param context The execution context used for dispatching.
 * @param out Optional output parameter to be re-used.
 * @return array The array holding the element-wise confined values.
 *
 * @note The bounds are arrays rather than scalars, so they broadcast
 * against the value and may vary across it. Complex arrays are
 * not accepted, there being no ordering to clamp against.
 */
XMIPP4_CORE_API
array clip(
	const_array_ref x,
	const_array_ref lower,
	const_array_ref upper,
	const execution_context &context,
	array *out = nullptr
);

} // namespace xmipp4
