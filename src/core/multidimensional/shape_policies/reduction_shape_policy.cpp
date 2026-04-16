// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/shape_policies/reduction_shape_policy.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <algorithm>

namespace xmipp4
{
namespace multidimensional
{

reduction_shape_policy::reduction_shape_policy(
	span<const std::size_t> reduction_axes
) noexcept
	: m_reduction_axes(reduction_axes)
{
}

reduction_shape_policy::~reduction_shape_policy() = default;

void reduction_shape_policy::infer_output(
	span<strided_layout> output_layouts,
	span<strided_layout> input_layouts
) const
{
	if (input_layouts.empty())
	{
		throw std::invalid_argument(
			"reduction_shape_policy::infer_output requires at least one input"
		);
	}

	std::vector<std::size_t> broadcasted_extents;
	input_layouts[0].get_extents(broadcasted_extents);

	std::vector<std::size_t> extents;
	for (std::size_t i = 1; i < input_layouts.size(); ++i)
	{
		input_layouts[i].get_extents(extents);
		broadcast_extents(broadcasted_extents, extents);
	}

	auto reduced_extents = broadcasted_extents;
	for (auto reduction_axis : m_reduction_axes)
	{
		reduced_extents.at(reduction_axis) = 1;
	}

	const auto physical_layout = 
		strided_layout::make_contiguous_layout(make_span(reduced_extents));
	std::fill(
		output_layouts.begin(),
		output_layouts.end(),
		physical_layout.broadcast_to(make_span(broadcasted_extents))
	);

	for (auto &layout : input_layouts)
	{
		layout = layout.broadcast_to(make_span(broadcasted_extents));
	}
}

void reduction_shape_policy::validate(
	span<const strided_layout> output_layouts,
	span<strided_layout> input_layouts
) const
{
	if (input_layouts.empty())
	{
		throw std::invalid_argument(
			"reduction_shape_policy::validate requires at least one output"
		);
	}

	std::vector<std::size_t> output_extents;
	output_layouts[0].get_extents(output_extents);
	for (std::size_t i = 1; i < output_layouts.size(); ++i)
	{
		if(!output_layouts[i].extents_equal(make_span(output_extents)))
		{
			throw std::invalid_argument(
				"multi-output reduction_shape_policy::validate requires all "
				"outputs to have the same extents."
			);
		}
	}

	for (auto &layout : input_layouts)
	{
		layout = layout.broadcast_to(make_span(output_extents));
	}


	std::vector<std::ptrdiff_t> strides;
	for (const auto &layout : output_layouts)
	{
		layout.get_strides(strides);
		XMIPP4_ASSERT( strides.size() == output_extents.size() );
		for (std::size_t axis = 0; axis < strides.size(); ++axis)
		{
			const auto is_reduction_axis =
				this->is_reduction_axis(axis);
			const auto is_broadcastable =
				output_extents[axis] > 1;
			const auto is_broadcasted =
				strides[axis] == 0;
			if (is_reduction_axis && is_broadcastable && !is_broadcasted)
			{
				throw std::invalid_argument(
					"reduction_shape_policy::validate: reduction axis does not "
					"have a zero stride in the output"
				);
			}
			if (!is_reduction_axis && is_broadcastable && is_broadcasted)
			{
				throw std::invalid_argument(
					"reduction_shape_policy::validate: non-reduction axis has "
					"a zero stride in the output"
				);
			}
		}
	}
}

bool reduction_shape_policy::is_reduction_axis(std::size_t axis) const noexcept
{
	const auto ite = 
		std::find(m_reduction_axes.begin(), m_reduction_axes.end(), axis);
	return ite != m_reduction_axes.end();
}

} // namespace multidimensional
} // namespace xmipp4
