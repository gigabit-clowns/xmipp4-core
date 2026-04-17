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
 * @brief Evaluate `min(x, y)` for each element in the arrays.
 *
 * Operates on integer and floating-point types. Complex types are not
 * supported.
 *
 * @param x First operand.
 * @param y Second operand.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The element-wise minimum of x and y.
 */
XMIPP4_CORE_API
array minimum(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out = nullptr
);

/**
 * @brief Evaluate `max(x, y)` for each element in the arrays.
 *
 * Operates on integer and floating-point types. Complex types are not
 * supported.
 *
 * @param x First operand.
 * @param y Second operand.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The element-wise maximum of x and y.
 */
XMIPP4_CORE_API
array maximum(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out = nullptr
);

/**
 * @brief Evaluate `clamp(x, lo, hi) = min(max(x, lo), hi)` for each element.
 *
 * Operates on integer and floating-point types. Complex types are not
 * supported.
 *
 * @param x Input array.
 * @param lo Lower bound array.
 * @param hi Upper bound array.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be re-used
 * and it will be overwritten with the newly created array.
 * @return array The element-wise clamped values.
 */
XMIPP4_CORE_API
array clamp(
    array_view x,
    array_view lo,
    array_view hi,
    const execution_context &context,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
