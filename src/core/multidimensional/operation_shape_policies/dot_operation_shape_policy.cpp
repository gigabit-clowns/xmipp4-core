// SPDX-License-Identifier: GPL-3.0-only

#include "dot_operation_shape_policy.hpp"

#include "operation_shape_policy_helpers.hpp"

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void dot_operation_shape_policy::deduce_output(
	span<shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT(output_shapes.size() == 1);
	output_shapes[0] = {};
}

void dot_operation_shape_policy::validate(
	span<const shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT(output_shapes.size() == 1);
	XMIPP4_ASSERT(input_shapes.size() == 2);

	if (output_shapes[0].size() != 0)
	{
		throw std::invalid_argument(
			"dot_operation_shape_policy expected scalar output."
		);
	}

	require_1d(input_shapes[0], "dot_operation_shape_policy", "input");
	if ( input_shapes[0] != input_shapes[1] )
	{
		throw std::invalid_argument(
			"dot_operation_shape_policy expected operands with equal lengths."
		);
	}
}

const dot_operation_shape_policy& dot_operation_shape_policy::get() noexcept
{
    static const dot_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
