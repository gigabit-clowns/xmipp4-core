// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Shape policy for elementwise operations.
 * 
 * After validation/inference all operators will have tha same shape through
 * broadcasting. 
 * 
 * `infer_output` finds the "consensus" shape of the inputs
 * and broadcasts them to this shape. Outputs are assigned a contiguous
 * layout with of this consensus shape.
 * 
 * `validate` broadcasts all inputs to the shape of the output. If multiple
 * outputs are provided, all must have matching shapes.
 */
class XMIPP4_CORE_API elementwise_shape_policy final
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

	const elementwise_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4

