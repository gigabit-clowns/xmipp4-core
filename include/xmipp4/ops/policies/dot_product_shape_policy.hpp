// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/dispatch/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace ops
{

/**
 * @brief Shape policy for a sum of products over one contracted axis.
 *
 * The last axis of the first operand is contracted with the second to last
 * of the second, and every remaining axis of both is kept, in that order:
 * `[a..., k]` against `[b..., k, c]` gives `[a..., b..., c]`. Two vectors
 * therefore give a scalar.
 *
 * This is where it parts company with a matrix multiplication, which the
 * two agree with only for one and two dimensional operands. Beyond that a
 * matrix multiplication broadcasts the leading axes as a stack of
 * independent matrices, while this keeps every one of them, so the result
 * grows rather than staying the same rank.
 *
 * Stateless, and so shared by every instance.
 *
 * @see matrix_multiply_shape_policy
 */
class XMIPP4_CORE_API dot_product_shape_policy final
	: public operation_shape_policy
{
public:
	void deduce(
		const operation_descriptor &descriptor,
		span<shape_type> canonical_output_shapes,
		span<const shape_type> input_shapes
	) const override;

	static const dot_product_shape_policy& get() noexcept;
};

} // namespace ops
} // namespace xmipp4
