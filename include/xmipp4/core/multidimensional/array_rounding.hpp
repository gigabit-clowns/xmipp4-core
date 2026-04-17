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
 * @brief Evaluate `floor(x)` for each element in the array.
 *
 * Returns the largest integer value not greater than x.
 * Operates on floating-point types only.
 *
 * @param x Input array.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The floor of each element.
 */
XMIPP4_CORE_API
array floor(
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

/**
 * @brief Evaluate `ceil(x)` for each element in the array.
 *
 * Returns the smallest integer value not less than x.
 * Operates on floating-point types only.
 *
 * @param x Input array.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The ceiling of each element.
 */
XMIPP4_CORE_API
array ceil(
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

/**
 * @brief Evaluate `round(x)` for each element in the array.
 *
 * Rounds to the nearest integer, with ties rounding away from zero.
 * Operates on floating-point types only.
 *
 * @param x Input array.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The rounded value of each element.
 */
XMIPP4_CORE_API
array round(
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

/**
 * @brief Evaluate `trunc(x)` for each element in the array.
 *
 * Rounds towards zero (truncates the fractional part).
 * Operates on floating-point types only.
 *
 * @param x Input array.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The truncated value of each element.
 */
XMIPP4_CORE_API
array trunc(
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
