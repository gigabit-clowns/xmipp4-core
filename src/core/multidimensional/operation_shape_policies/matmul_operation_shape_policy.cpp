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

void matmul_operation_shape_policy::deduce_output(
	span<shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT( output_shapes.size() == 1 );
	XMIPP4_ASSERT( input_shapes.size() == 2 );
	
	auto first_extents = input_shapes[0];
	require_at_least_2d(
		first_extents, 
		"matmul_operation_shape_policy", 
		"first input"
	);
	const auto batch_rank1 = first_extents.size() - 2;
	const auto n = first_extents[batch_rank1 + 0];

	auto second_extents = input_shapes[0];
	require_at_least_2d(
		second_extents, 
		"matmul_operation_shape_policy", 
		"second input"
	);
	const auto batch_rank2 = second_extents.size() - 2;
	const auto m = second_extents[batch_rank2 + 1];

	first_extents.resize(batch_rank1);
	second_extents.resize(batch_rank2);
	broadcast_extents(first_extents, second_extents);

	auto canonical_extents = std::move(first_extents);
	canonical_extents.emplace_back(n);
	canonical_extents.emplace_back(m);

	output_shapes[0] = std::move(canonical_extents);
}

void matmul_operation_shape_policy::validate(
	span<const shape_type> output_shapes,
	span<const shape_type> input_shapes
) const
{
	XMIPP4_ASSERT( output_shapes.size() == 1 );
	XMIPP4_ASSERT( input_shapes.size() == 2 );
	
	const auto &output_shape = output_shapes[0];
	require_at_least_2d(
		output_shape, 
		"matmul_operation_shape_policy", 
		"output"
	);
	const auto output_batch_rank = output_shape.size() - 2;
	const auto n = output_shape[output_batch_rank+0];
	const auto m = output_shape[output_batch_rank+1];
	const span<const std::size_t> ouput_batch_shape(
		output_shape.data(), 
		output_batch_rank
	);

	const auto &first_input_shape = input_shapes[0];
	require_at_least_2d(
		first_input_shape, 
		"matmul_operation_shape_policy", 
		"first input"
	);
	const auto first_input_batch_rank = first_input_shape.size() - 2;
	const auto n1 = first_input_shape[first_input_batch_rank+0];
	const auto k1 = first_input_shape[first_input_batch_rank+1];
	const span<const std::size_t> first_input_batch_shape(
		first_input_shape.data(),
		first_input_batch_rank
	);
	if (!is_broadcastable_to(first_input_batch_shape, ouput_batch_shape))
	{
		throw std::invalid_argument(
			"matmul_operation_shape_policy requires first operand's batch "
			"dimensions to be broadcastable to output's batch dimensions."
		);
	}
	if (n1 != n)
	{
		throw std::invalid_argument(
			"matmul_operation_shape_policy requires first operand's row count "
			"to match output operand's row count."
		);
	}

	const auto &second_input_shape = input_shapes[1];
	require_at_least_2d(
		second_input_shape, 
		"matmul_operation_shape_policy", 
		"second input"
	);
	const auto second_input_batch_rank = second_input_shape.size() - 2;
	const auto k2 = second_input_shape[second_input_batch_rank+0];
	const auto m2 = second_input_shape[second_input_batch_rank+1];
	const span<const std::size_t> second_input_batch_shape(
		second_input_shape.data(),
		second_input_batch_rank
	);
	if (!is_broadcastable_to(second_input_batch_shape, ouput_batch_shape))
	{
		throw std::invalid_argument(
			"matmul_operation_shape_policy requires second operand's batch "
			"dimensions to be broadcastable to output's batch dimensions."
		);
	}
	if (m2 != m)
	{
		throw std::invalid_argument(
			"matmul_operation_shape_policy requires second operand's column "
			"count to match output operand's column count."
		);
	}
	if (k1 != k2)
	{
		throw std::invalid_argument(
			"matmul_operation_shape_policy requires first operand's column "
			"count to match second operand's row count."
		);
	}
}

const matmul_operation_shape_policy& 
matmul_operation_shape_policy::get() noexcept
{
    static const matmul_operation_shape_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
