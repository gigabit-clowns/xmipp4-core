// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>

#include <xmipp4/core/multidimensional/broadcast.hpp>

namespace xmipp4
{
namespace multidimensional
{

void elementwise_operation_shape_policy::deduce_output(
	span<shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (input_shapes.empty())
	{
		throw std::invalid_argument(
			"elementwise_operation_shape_policy requires at least one input "
			"when deducing outputs."
		);
	}

	shape_type canonical_shape = input_shapes[0];
	for (std::size_t i = 1; i < input_shapes.size(); ++i)
	{
		broadcast_extents_accumulate(
			canonical_shape, 
			make_span(input_shapes[i])
		);
	}

	std::fill(
		output_shapes.begin(),
		output_shapes.end(),
		std::move(canonical_shape)
	);
}

void elementwise_operation_shape_policy::validate(
	span<const shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (output_shapes.empty())
	{
		throw std::invalid_argument(
			"elementwise_operation_shape_policy requires at least one output."
		);
	}

	const auto &reference_shape = output_shapes[0];
	for (std::size_t i = 1; i < output_shapes.size(); ++i)
	{
		if(output_shapes[i] != reference_shape)
		{
			throw std::invalid_argument(
				"elementwise_operation_shape_policy requires all outputs to "
				"have the same shape."
			);
		}
	}

	for (const auto &input_shape : input_shapes)
	{
		const auto valid = is_broadcastable_to(
			make_span(input_shape), 
			make_span(reference_shape)
		);

		if(!valid)
		{
			throw std::invalid_argument(
				"elementwise_operation_shape_policy requires all inputs to be "
				"broadcastable to the output shape."
			);
		}
	}
}

const elementwise_operation_shape_policy& 
elementwise_operation_shape_policy::get() noexcept
{
	static const elementwise_operation_shape_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
