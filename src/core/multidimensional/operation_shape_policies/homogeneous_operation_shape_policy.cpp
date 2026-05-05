// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_shape_policies/homogeneous_operation_shape_policy.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void homogeneous_operation_shape_policy::deduce(
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (input_shapes.empty())
	{
		throw std::invalid_argument(
			"homogeneous_operation_shape_policy requires at least one input."
		);
	}

	const auto &reference_shape = input_shapes[0];
	for (std::size_t i = 1; i < input_shapes.size(); ++i)
	{
		if (input_shapes[i] != reference_shape)
		{
			throw std::invalid_argument(
				"homogeneous_operation_shape_policy requires all inputs to "
				"have the same shape."
			);
		}
	}

	std::fill(
		canonical_output_shapes.begin(),
		canonical_output_shapes.end(),
		reference_shape
	);
}

const homogeneous_operation_shape_policy&
homogeneous_operation_shape_policy::get() noexcept
{
	static const homogeneous_operation_shape_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
