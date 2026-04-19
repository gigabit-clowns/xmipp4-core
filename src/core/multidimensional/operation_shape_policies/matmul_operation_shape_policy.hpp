// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Shape policy for matrix-matrix multiplication.
 *
 * Supports batched multiplication. The last two axes are the matrix dimensions;
 * all preceding axes are batch dimensions and are broadcast-matched.
 *
 * input[0]: (..., m, k)
 * input[1]: (..., k, n)
 * output:   (..., m, n)
 *
 * `infer_output` checks that the inner dimensions match (k), broadcasts batch
 * dimensions to their consensus shape, and assigns a contiguous output layout.
 *
 * `validate` verifies that input and output shapes are mutually consistent.
 */
class XMIPP4_CORE_API matmul_operation_shape_policy final
    : public operation_shape_policy
{
public:
    void infer_output(
        span<strided_layout> output_layouts,
        span<strided_layout> input_layouts
    ) const override;

    void validate(
        span<const strided_layout> output_layouts,
        span<strided_layout> input_layouts
    ) const override;

    static const matmul_operation_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
