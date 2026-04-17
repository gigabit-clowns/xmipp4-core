// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/shape_policies/same_shape_policy.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>

namespace xmipp4
{
namespace multidimensional
{

void same_shape_policy::infer_output(
	span<strided_layout> output_layouts,
	span<strided_layout> input_layouts
) const
{
	if (input_layouts.empty())
	{
		throw std::invalid_argument(
			"same_shape_policy::infer_output requires at least one input."
		);
	}

	std::vector<std::size_t> extents;
	input_layouts[0].get_extents(extents);

	for (std::size_t i = 1; i < input_layouts.size(); ++i)
	{
		if (!input_layouts[i].extents_equal(make_span(extents)))
		{
			throw std::invalid_argument(
				"same_shape_policy::infer_output requires all inputs to have "
				"the same shape."
			);
		}
	}

	std::fill(
		output_layouts.begin(),
		output_layouts.end(),
		strided_layout::make_contiguous_layout(make_span(extents))
	);
}

void same_shape_policy::validate(
	span<const strided_layout> output_layouts,
	span<strided_layout> input_layouts
) const
{
	if (output_layouts.empty())
	{
		throw std::invalid_argument(
			"same_shape_policy::validate requires at least one output"
		);
	}

	std::vector<std::size_t> extents;
	output_layouts[0].get_extents(extents);
	for (std::size_t i = 1; i < output_layouts.size(); ++i)
	{
		if(!output_layouts[i].extents_equal(make_span(extents)))
		{
			throw std::invalid_argument(
				"same_shape_policy::validate requires all outputs to have the "
				"same extents."
			);
		}
	}

	for (auto &layout : input_layouts)
	{
		if (!layout.extents_equal(make_span(extents)))
		{
			throw std::invalid_argument(
				"same_shape_policy::validate requires inputs to have the same "
				"shape as the outputs."
			);
		}
	}
}

const same_shape_policy& same_shape_policy::get() noexcept
{
	static const same_shape_policy instance;
	return instance;
}


} // namespace multidimensional
} // namespace xmipp4
