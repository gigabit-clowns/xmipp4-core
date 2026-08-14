// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/policies/sequence_operation_shape_policy.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace xmipp4
{
namespace ops
{

sequence_operation_shape_policy::sequence_operation_shape_policy(
	std::size_t count
) noexcept
	: m_count(count)
{
}

std::size_t sequence_operation_shape_policy::get_count() const noexcept
{
	return m_count;
}

void sequence_operation_shape_policy::deduce(
	const operation_descriptor &descriptor,
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (!input_shapes.empty())
	{
		std::ostringstream oss;
		oss << descriptor << ": a sequence is written from its parameters "
			<< "alone and takes no input operand.";
		throw std::invalid_argument(oss.str());
	}

	std::fill(
		canonical_output_shapes.begin(),
		canonical_output_shapes.end(),
		shape_type{ m_count }
	);
}

} // namespace ops
} // namespace xmipp4
