// SPDX-License-Identifier: GPL-3.0-only

#include "matvec_operation_shape_policy.hpp"

#include "operation_shape_policy_helpers.hpp"

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void matvec_operation_shape_policy::deduce_output(
	span<shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT( output_shapes.size() == 1 );
	XMIPP4_ASSERT( input_shapes.size() == 2 );

	require_2d(input_shapes[0], "matvec_operation_shape_policy", "first input");
	output_shapes[0] = { input_shapes[0][0] };
}

void matvec_operation_shape_policy::validate(
	span<const shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT(output_shapes.size() == 1);
	XMIPP4_ASSERT(input_shapes.size() == 2);

	const auto &output_shape = output_shapes[0];
	require_1d(output_shape, "matvec_operation_shape_policy", "output");
	const auto n = output_shape[0];

	const auto &first_input_shape = input_shapes[0];
	require_2d(
		first_input_shape, 
		"matvec_operation_shape_policy", 
		"first input"
	);
	const auto n1 = first_input_shape[0];
	const auto m1 = first_input_shape[1];
	if (n != n1)
	{
		throw std::invalid_argument(
			"matvec_operation_shape_policy requires output operand's length to "
			"be the same as the row count of the first operand."
		);
	}

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
}

const matvec_operation_shape_policy& matvec_operation_shape_policy::get() noexcept
{
    static const matvec_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
