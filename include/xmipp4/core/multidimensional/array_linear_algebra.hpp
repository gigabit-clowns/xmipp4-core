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
 * @brief Compute the matrix-matrix product C = A * B.
 *
 * Supports batched multiplication over leading dimensions.
 * The last two axes of each input are the matrix dimensions; all preceding
 * axes are batch dimensions and are broadcast-matched.
 *
 * @param a Left-hand operand of shape (..., m, k). Must have rank >= 2.
 * @param b Right-hand operand of shape (..., k, n). Must have rank >= 2.
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be
 * re-used and it will be overwritten with the result.
 * @return array The result of shape (..., m, n).
 */
XMIPP4_CORE_API
array matmul(
    array_view a,
    array_view b,
    const execution_context &context,
    array *out = nullptr
);

/**
 * @brief Compute the matrix-vector product y = A * x.
 *
 * @param a Matrix operand of shape (m, n).
 * @param x Vector operand of shape (n,).
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be
 * re-used and it will be overwritten with the result.
 * @return array The result of shape (m,).
 */
XMIPP4_CORE_API
array matvec(
    array_view a,
    array_view x,
    const execution_context &context,
    array *out = nullptr
);

/**
 * @brief Compute the dot product of two 1D vectors: sum(x * y).
 *
 * @param x First vector of shape (n,).
 * @param y Second vector of shape (n,).
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be
 * re-used and it will be overwritten with the result.
 * @return array A scalar (rank-0) array containing the dot product value.
 */
XMIPP4_CORE_API
array dot(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out = nullptr
);

/**
 * @brief Compute the outer product of two 1D vectors: result[i, j] = x[i] * y[j].
 *
 * @param x First vector of shape (m,).
 * @param y Second vector of shape (n,).
 * @param context The device context to handle the allocation.
 * @param out Optional array to reuse. If provided, its resources may be
 * re-used and it will be overwritten with the result.
 * @return array The result matrix of shape (m, n).
 */
XMIPP4_CORE_API
array outer(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
