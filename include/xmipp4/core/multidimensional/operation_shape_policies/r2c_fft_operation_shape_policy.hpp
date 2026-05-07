// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <cstddef>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Shape policy for real-to-complex FFT operations.
 *
 * All inputs must share the same shape. The output shape is identical to the
 * input shape except that the extent at @p transformed_axis is compressed
 * from @c N to @c N/2+1 (integer division), yielding the non-redundant
 * half-spectrum of a real-input FFT. All outputs receive the same shape.
 *
 * input[i]:  (..., N, ...)     — extent N at transformed_axis
 * output[j]: (..., N/2+1, ...) — same position
 */
class XMIPP4_CORE_API r2c_fft_operation_shape_policy final
    : public operation_shape_policy
{
public:
    /**
     * @brief Construct the policy.
     *
     * @param transformed_axis Index of the axis along which the
     * real-to-complex conversion is performed. This axis will have its
     * extent compressed to @c N/2+1 in the output.
     */
    explicit r2c_fft_operation_shape_policy(
        std::size_t transformed_axis
    ) noexcept;

    ~r2c_fft_operation_shape_policy() override;

    void deduce(
        span<shape_type> canonical_output_shapes,
        span<const shape_type> input_shapes
    ) const override;

    /**
     * @brief Return the index of the transformed axis.
     */
    std::size_t get_transformed_axis() const noexcept;

private:
    std::size_t m_transformed_axis;
};

} // namespace multidimensional
} // namespace xmipp4
