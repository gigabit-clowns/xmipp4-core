// SPDX-License-Identifier: GPL-3.0-only

#include "matvec_operation_shape_policy.hpp"

#include "operation_shape_policy_helpers.hpp"

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void matvec_operation_shape_policy::deduce(
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT( canonical_output_shapes.size() == 1 );
	XMIPP4_ASSERT( input_shapes.size() == 2 );

	const auto &first_input_shape = input_shapes[0];
	require_2d(
		first_input_shape,
		"matvec_operation_shape_policy",
		"first input"
	);
	const auto n = first_input_shape[0];
	const auto m1 = first_input_shape[1];

	const auto &second_input_shape = input_shapes[1];
	require_1d(
		second_input_shape,
		"matvec_operation_shape_policy",
		"second input"
	);
	const auto m2 = second_input_shape[0];
	if (m1 != m2)
	{
		throw std::invalid_argument(
			"matvec_operation_shape_policy requires second operand's length to "
			"be the same as the column count of the first operand."
		);
	}

	canonical_output_shapes[0] = { n };
}

const matvec_operation_shape_policy& matvec_operation_shape_policy::get() noexcept
{
    static const matvec_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
