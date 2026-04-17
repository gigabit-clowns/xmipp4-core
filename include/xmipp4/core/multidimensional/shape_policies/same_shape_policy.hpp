// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Shape policy to ensure that all operands have the same shape.
 * 
 * After validation/inference all operators will have tha same shape through
 * broadcasting. 
 * 
 * `infer_output` Requires all inputs to have the same shape and assigns 
 * this shape to the output.
 * 
 * `validate` Ensures that all outputs and inputs have the same shape.
 */
class XMIPP4_CORE_API same_shape_policy final
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

	static const same_shape_policy& get() noexcept;
};

} // namespace multidimensional
} // namespace xmipp4

