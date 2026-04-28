// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
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
class XMIPP4_CORE_API dot_operation_shape_policy final
    : public operation_shape_policy
{
public:
	void deduce_output(
		span<shape_type> output_shapes,
		span<const shape_type> input_shapes
	) const override;

	void validate(
		span<const shape_type> output_shapes,
		span<const shape_type> input_shapes
	) const override;

    static const dot_operation_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
