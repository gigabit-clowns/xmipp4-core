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
 * @brief Evaluate `sin(x)` for each angle in the input array.
 *
 * @param x The angles in radians. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting sine.
 */
XMIPP4_CORE_API
array sin(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `cos(x)` for each angle in the input array.
 *
 * @param x The angles in radians. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting cosine.
 */
XMIPP4_CORE_API
array cos(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `sin(x)` and `cos(x)` simultaneously for each angle in the
 * input array.
 *
 * @param x The angles in radians. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 */
XMIPP4_CORE_API
void sincos(
	array_view x,
	const execution_context &context,
	array &sin,
	array &cos
);

/**
 * @brief Evaluate `tan(x)` for each angle in the input array.
 *
 * @param x The angles in radians. 
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting tangent.
 */
XMIPP4_CORE_API
array tan(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `sin^-1(x)` (arcsin) for each value in the input array.
 *
 * @param x The sine values (must be in [-1, 1]).
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting angles in radians.
 */
XMIPP4_CORE_API
array asin(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `cos^-1(x)` (arccos) for each value in the input array.
 *
 * @param x The cosine values (must be in [-1, 1]).
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting angles in radians.
 */
XMIPP4_CORE_API
array acos(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `tan^-1(x)` (arctan) for each value in the input array.
 *
 * @param x The tangent values (must be in [-1, 1]).
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting angles in radians.
 */
XMIPP4_CORE_API
array atan(
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

/**
 * @brief Evaluate `tan^-1(y / x)` (arctan) for each value in the input array,
 * with the appropriate quadrant.
 *
 * @param y The value in the vertical axis.
 * @param x The value in the horizontal axis.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The resulting angles in radians.
 */
XMIPP4_CORE_API
array atan(
	array_view y,
	array_view x,
	const execution_context &context,
	array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
