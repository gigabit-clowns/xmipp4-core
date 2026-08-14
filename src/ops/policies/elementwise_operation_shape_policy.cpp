// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/policies/elementwise_operation_shape_policy.hpp>

#include <xmipp4/core/layout/broadcast.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include <sstream>
#include <stdexcept>

namespace xmipp4
{
namespace ops
{

void elementwise_operation_shape_policy::deduce(
	const operation_descriptor& /*descriptor*/,
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (input_shapes.empty())
	{
		std::fill(
			canonical_output_shapes.begin(),
			canonical_output_shapes.end(),
			shape_type()
		);
		return;
	}

	shape_type canonical_shape = input_shapes[0];
	for (std::size_t i = 1; i < input_shapes.size(); ++i)
	{
		broadcast_extents_accumulate(
			canonical_shape,
			make_span(input_shapes[i])
		);
	}

	if (!canonical_output_shapes.empty())
	{
		std::fill(
			canonical_output_shapes.begin(),
			std::prev(canonical_output_shapes.end()),
			canonical_shape
		);
		canonical_output_shapes.back() = std::move(canonical_shape);
	}
}

void elementwise_operation_shape_policy::accept(
	const operation_descriptor &descriptor,
	span<const shape_type> user_output_shapes,
	span<const shape_type> canonical_output_shapes,
	span<const shape_type> /*input_shapes*/
) const
{
	XMIPP4_ASSERT(
		user_output_shapes.size() == canonical_output_shapes.size()
	);

	if (user_output_shapes.empty())
	{
		return;
	}

	const auto &reference_shape = user_output_shapes[0];
	for (std::size_t i = 1; i < user_output_shapes.size(); ++i)
	{
		if (user_output_shapes[i] != reference_shape)
		{
			std::ostringstream oss;
			oss << descriptor << ": output operand "
				<< describe_operand(descriptor, i, true)
				<< " does not have the same shape as output operand "
				<< describe_operand(descriptor, 0, true)
				<< ", which an elementwise operation requires.";
			throw std::invalid_argument(oss.str());
		}
	}

	const auto valid = is_broadcastable_to(
		make_span(canonical_output_shapes[0]),
		make_span(reference_shape)
	);
	if (!valid)
	{
		std::ostringstream oss;
		oss << descriptor << ": output operand "
			<< describe_operand(descriptor, 0, true)
			<< " has a shape the inputs cannot be broadcast to.";
		throw std::invalid_argument(oss.str());
	}
}

const elementwise_operation_shape_policy&
elementwise_operation_shape_policy::get() noexcept
{
	static const elementwise_operation_shape_policy instance;
	return instance;
}

} // namespace ops
} // namespace xmipp4
