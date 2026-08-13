// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/policies/axiswise_operation_shape_policy.hpp>

#include <xmipp4/core/layout/broadcast.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace ops
{

axiswise_operation_shape_policy::axiswise_operation_shape_policy(
	axis_list axes
)
	: m_axes(make_sorted_axis_list(std::move(axes)))
{
}

span<const std::size_t>
axiswise_operation_shape_policy::get_axes() const noexcept
{
	return make_span(m_axes.data(), m_axes.size());
}

void axiswise_operation_shape_policy::deduce(
	const operation_descriptor &descriptor,
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (input_shapes.empty())
	{
		std::ostringstream oss;
		oss << descriptor << ": at least one input operand is needed to "
			<< "take a shape from.";
		throw std::invalid_argument(oss.str());
	}

	shape_type shape = input_shapes[0];
	for (std::size_t i = 1; i < input_shapes.size(); ++i)
	{
		broadcast_extents_accumulate(shape, make_span(input_shapes[i]));
	}

	check_axes_within_rank(descriptor, get_axes(), shape.size());

	if (!canonical_output_shapes.empty())
	{
		std::fill(
			canonical_output_shapes.begin(),
			std::prev(canonical_output_shapes.end()),
			shape
		);
		canonical_output_shapes.back() = std::move(shape);
	}
}

} // namespace ops
} // namespace xmipp4
