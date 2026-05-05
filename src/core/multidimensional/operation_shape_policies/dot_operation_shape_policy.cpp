// SPDX-License-Identifier: GPL-3.0-only

#include "dot_operation_shape_policy.hpp"

#include "operation_shape_policy_helpers.hpp"

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void dot_operation_shape_policy::deduce(
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT(canonical_output_shapes.size() == 1);
	XMIPP4_ASSERT(input_shapes.size() == 2);

	require_1d(input_shapes[0], "dot_operation_shape_policy", "first input");
	require_1d(input_shapes[1], "dot_operation_shape_policy", "second input");
	if (input_shapes[0] != input_shapes[1])
	{
		throw std::invalid_argument(
			"dot_operation_shape_policy expected operands with equal lengths."
		);
	}

	canonical_output_shapes[0] = {};
}

const dot_operation_shape_policy& dot_operation_shape_policy::get() noexcept
{
    static const dot_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
