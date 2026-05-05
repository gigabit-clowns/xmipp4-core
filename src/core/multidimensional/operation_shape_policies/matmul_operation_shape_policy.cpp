// SPDX-License-Identifier: GPL-3.0-only

#include "matmul_operation_shape_policy.hpp"

#include "operation_shape_policy_helpers.hpp"

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/broadcast.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void matmul_operation_shape_policy::deduce(
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT( canonical_output_shapes.size() == 1 );
	XMIPP4_ASSERT( input_shapes.size() == 2 );

	auto first_extents = input_shapes[0];
	require_at_least_2d(
		first_extents,
		"matmul_operation_shape_policy",
		"first input"
	);
	const auto batch_rank1 = first_extents.size() - 2;
	const auto n = first_extents[batch_rank1 + 0];
	const auto k1 = first_extents[batch_rank1 + 1];

	auto second_extents = input_shapes[1];
	require_at_least_2d(
		second_extents,
		"matmul_operation_shape_policy",
		"second input"
	);
	const auto batch_rank2 = second_extents.size() - 2;
	const auto k2 = second_extents[batch_rank2 + 0];
	const auto m = second_extents[batch_rank2 + 1];

	if (k1 != k2)
	{
		throw std::invalid_argument(
			"matmul_operation_shape_policy requires first operand's column "
			"count to match second operand's row count."
		);
	}

	first_extents.resize(batch_rank1);
	second_extents.resize(batch_rank2);
	broadcast_extents(first_extents, second_extents);

	auto canonical_extents = std::move(first_extents);
	canonical_extents.emplace_back(n);
	canonical_extents.emplace_back(m);

	canonical_output_shapes[0] = std::move(canonical_extents);
}

const matmul_operation_shape_policy& 
matmul_operation_shape_policy::get() noexcept
{
    static const matmul_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
