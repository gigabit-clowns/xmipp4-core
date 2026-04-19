// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Shape policy for the outer product of two 1D vectors.
 *
 * input[0]: (m,)   - first 1D vector
 * input[1]: (n,)   - second 1D vector
 * output:   (m, n) - 2D result matrix
 *
 * Both inputs must be exactly 1D.
 */
class XMIPP4_CORE_API outer_operation_shape_policy final
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

    static const outer_operation_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
