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
 * @brief Shape policy for complex-to-real (inverse real) FFT operations.
 *
 * All inputs must share the same shape. The output shape is identical to the
 * input shape except that the extent at @p transformed_axis is expanded
 * from @c M = @c N/2+1 back to the full real size @c N. All outputs receive
 * the same shape.
 *
 * input[i]:  (..., M, ...)   — extent M = N/2+1 at transformed_axis
 * output[j]: (..., N, ...)   — extent N at the same position
 *
 * @par Why is_output_odd and not a full output size
 * For a given half-spectrum size @c M, the only two valid real output sizes
 * are @c 2*(M-1) (even) and @c 2*M-1 (odd). A boolean flag selects exactly
 * between these two cases and admits no other (invalid) value. Storing the
 * full @c N instead would allow values such as @c 2*(M-1)+2 that can never
 * arise from an r2c transform of that input.
 */
class XMIPP4_CORE_API c2r_fft_operation_shape_policy final
    : public operation_shape_policy
{
public:
    /**
     * @brief Construct the policy.
     *
     * @param transformed_axis Index of the axis along which the
     * complex-to-real conversion is performed.
     * @param is_output_odd True if the full real output size @c N along
     * @p transformed_axis is odd, i.e. @c N = 2*M-1; false if it is
     * even, i.e. @c N = 2*(M-1), where @c M is the input extent at that axis.
     */
    explicit c2r_fft_operation_shape_policy(
        std::size_t transformed_axis,
        bool is_output_odd
    ) noexcept;

    ~c2r_fft_operation_shape_policy() override;

    void deduce(
        span<shape_type> canonical_output_shapes,
        span<const shape_type> input_shapes
    ) const override;

    /**
     * @brief Return the index of the transformed axis.
     */
    std::size_t get_transformed_axis() const noexcept;

    /**
     * @brief Return whether the real output size along the transformed
     * axis is odd.
     */
    bool is_output_odd() const noexcept;

	/**
	 * @brief Apply the policy to the transformed axis' extent.
	 * 
	 * The result of this operation is `(extent-1)*2 + is_output_odd`.
	 * 
	 * @param extent The extent of the transformed axis. Must be greater than
	 * zero. Otherwise, behavior is undefined.
	 * @return std::size_t The new extent.
	 */
	std::size_t apply_to(std::size_t extent) const noexcept;

private:
    std::size_t m_transformed_axis;
    bool m_is_output_odd;
};

} // namespace multidimensional
} // namespace xmipp4
