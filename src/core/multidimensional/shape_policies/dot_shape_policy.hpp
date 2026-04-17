// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Shape policy for the dot product of two 1D vectors.
 *
 * input[0]: (n,) - first 1D vector
 * input[1]: (n,) - second 1D vector (same length)
 * output:   ()   - scalar (rank 0)
 *
 * Both inputs must be 1D and have identical size.
 */
class XMIPP4_CORE_API dot_shape_policy final
    : public shape_policy
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

    static const dot_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
