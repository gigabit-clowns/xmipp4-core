// SPDX-License-Identifier: GPL-3.0-only

#include "outer_operation_shape_policy.hpp"

#include "operation_shape_policy_helpers.hpp"

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <stdexcept>
#include <array>

namespace xmipp4
{
namespace multidimensional
{

void outer_operation_shape_policy::deduce(
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT( canonical_output_shapes.size() == 1 );
	XMIPP4_ASSERT( input_shapes.size() == 2 );

	const auto &first_input_shape = input_shapes[0];
	require_1d(
		first_input_shape,
		"outer_operation_shape_policy",
		"first input"
	);
	const auto n = first_input_shape[0];

	const auto &second_input_shape = input_shapes[1];
	require_1d(
		second_input_shape,
		"outer_operation_shape_policy",
		"second input"
	);
	const auto m = second_input_shape[0];

	canonical_output_shapes[0] = { n, m };
}

const outer_operation_shape_policy& outer_operation_shape_policy::get() noexcept
{
    static const outer_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
