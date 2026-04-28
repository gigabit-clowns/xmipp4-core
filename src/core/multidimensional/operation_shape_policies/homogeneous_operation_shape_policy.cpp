// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_shape_policies/homogeneous_operation_shape_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

void homogeneous_operation_shape_policy::deduce_output(
	span<shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (input_shapes.empty())
	{
		throw std::invalid_argument(
			"homogeneous_operation_shape_policy requires at least one input "
			"when deducing outputs."
		);
	}

	std::fill(
		output_shapes.begin(),
		output_shapes.end(),
		input_shapes.front()
	);
}

void homogeneous_operation_shape_policy::validate(
	span<const shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (output_shapes.empty())
	{
		throw std::invalid_argument(
			"homogeneous_operation_shape_policy requires at least one output."
		);
	}

	const auto &reference_shape = output_shapes[0];
	for (std::size_t i = 1; i < output_shapes.size(); ++i)
	{
		if(output_shapes[i] != reference_shape)
		{
			throw std::invalid_argument(
				"homogeneous_operation_shape_policy requires all outputs to "
				"have the same shape."
			);
		}
	}

	for (const auto &input_shape : input_shapes)
	{
		if(input_shape != reference_shape)
		{
			throw std::invalid_argument(
				"homogeneous_operation_shape_policy requires all inputs to "
				"have the same shape."
			);
		}
	}
}

const homogeneous_operation_shape_policy& homogeneous_operation_shape_policy::get() noexcept
{
	static const homogeneous_operation_shape_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
