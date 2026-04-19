// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/multidimensional/operation_shape_policy.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Shape policy for reduction operations.
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
class XMIPP4_CORE_API reduction_operation_shape_policy final
	: public operation_shape_policy
{
public:
	/**
	 * @brief Construct a new reduction shape policy object. 
	 * 
	 * @param reduction_axes The axes where the reduction happens. Must outlive
	 * the constructed object.
	 */
	explicit reduction_operation_shape_policy(
		span<const std::size_t> reduction_axes
	) noexcept;
	~reduction_operation_shape_policy() override;

	void infer_output(
		span<strided_layout> output_layouts,
		span<strided_layout> input_layouts
	) const override;

	void validate(
		span<const strided_layout> output_layouts,
		span<strided_layout> input_layouts
	) const override;

private:
	span<const std::size_t> m_reduction_axes;

	bool is_reduction_axis(std::size_t axis) const noexcept;
};

} // namespace multidimensional
} // namespace xmipp4
