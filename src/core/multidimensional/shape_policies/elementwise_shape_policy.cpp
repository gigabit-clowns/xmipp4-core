// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>

namespace xmipp4
{
namespace multidimensional
{

void elementwise_shape_policy::infer_output(
	span<strided_layout> output_layouts,
	span<strided_layout> input_layouts
) const
{
	if (input_layouts.empty())
	{
		throw std::invalid_argument(
			"elementwise_shape_policy::infer_output requires at least one input"
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

	std::fill(
		output_layouts.begin(),
		output_layouts.end(),
		strided_layout::make_contiguous_layout(make_span(broadcasted_extents))
	);

	for (auto &layout : input_layouts)
	{
		layout = layout.broadcast_to(make_span(broadcasted_extents));
	}
}

void elementwise_shape_policy::validate(
	span<const strided_layout> output_layouts,
	span<strided_layout> input_layouts
) const
{
	if (input_layouts.empty())
	{
		throw std::invalid_argument(
			"elementwise_shape_policy::validate requires at least one output"
		);
	}

	std::vector<std::size_t> output_extents;
	output_layouts[0].get_extents(output_extents);
	for (std::size_t i = 1; i < output_layouts.size(); ++i)
	{
		if(!output_layouts[i].extents_equal(make_span(output_extents)))
		{
			throw std::invalid_argument(
				"multi-output elementwise_shape_policy::validate requires all "
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
		for (std::size_t i = 0; i < output_extents.size(); ++i)
		{
			if (output_extents[i] > 1 && strides[i] == 0)
			{
				throw std::invalid_argument(
					"elementwise_shape_policy::validate does not allow "
					"broadcasted output operands."
				);
			}
		}
	}
}

const elementwise_shape_policy& elementwise_shape_policy::get() noexcept
{
	static const elementwise_shape_policy instance;
	return instance;
}


} // namespace multidimensional
} // namespace xmipp4
