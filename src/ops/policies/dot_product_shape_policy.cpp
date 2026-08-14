// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/policies/dot_product_shape_policy.hpp>

#include "shape_deduction.hpp"

#include <iterator>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace ops
{

namespace
{

using shape_type = operation_shape_policy::shape_type;

void require_non_empty(
	const operation_descriptor &descriptor,
	const shape_type &shape,
	std::size_t index
)
{
	if (shape.empty())
	{
		std::ostringstream oss;
		oss << descriptor << ": input operand "
			<< describe_operand(descriptor, index, false)
			<< " has no axes, so there is nothing to contract.";
		throw std::invalid_argument(oss.str());
	}
}

} // anonymous namespace

void dot_product_shape_policy::deduce(
	const operation_descriptor &descriptor,
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (input_shapes.size() != 2)
	{
		std::ostringstream oss;
		oss << descriptor << ": a dot product takes exactly two input "
			<< "operands.";
		throw std::invalid_argument(oss.str());
	}

	const auto &left = input_shapes[0];
	const auto &right = input_shapes[1];
	require_non_empty(descriptor, left, 0);
	require_non_empty(descriptor, right, 1);

	// The last axis of the first operand meets the second to last of the
	// second, which for a vector is its only one.
	const auto contracted = left.back();
	const auto other = right.size() == 1
		? right.back()
		: right[right.size() - 2];

	if (contracted != other)
	{
		std::ostringstream oss;
		oss << descriptor << ": the contracted axis of input operand "
			<< describe_operand(descriptor, 0, false)
			<< " has extent " << contracted << ", but that of input "
			<< "operand " << describe_operand(descriptor, 1, false)
			<< " has extent " << other << ".";
		throw std::invalid_argument(oss.str());
	}

	// Every axis of both operands survives but the two contracted ones,
	// which is what sets this apart from a matrix multiplication.
	shape_type shape(left.begin(), std::prev(left.end()));
	if (right.size() > 1)
	{
		shape.insert(
			shape.end(),
			right.begin(),
			std::prev(right.end(), 2)
		);
		shape.push_back(right.back());
	}

	assign_output_shapes(canonical_output_shapes, std::move(shape));
}

const dot_product_shape_policy& dot_product_shape_policy::get() noexcept
{
	static const dot_product_shape_policy instance;
	return instance;
}

} // namespace ops
} // namespace xmipp4
