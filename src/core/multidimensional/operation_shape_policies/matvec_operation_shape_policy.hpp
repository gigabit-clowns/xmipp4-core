// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Shape policy for matrix-vector multiplication.
 *
 * input[0]: (m, n) - 2D matrix
 * input[1]: (n,)   - 1D vector
 * output:   (m,)   - 1D result vector
 *
 * Both input ranks are strictly enforced (2 and 1 respectively).
 */
class XMIPP4_CORE_API matvec_operation_shape_policy final
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

    static const matvec_operation_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
