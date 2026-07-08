// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/operations/shape_policies/elementwise_shape_policy.hpp>

#include <xmipp4/core/layout/broadcast.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace operations
{

void elementwise_shape_policy::deduce(
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
		layout::broadcast_extents_accumulate(
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

void elementwise_shape_policy::accept(
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
			throw std::invalid_argument(
				"elementwise_shape_policy requires all outputs to "
				"have the same shape."
			);
		}
	}

	const auto valid = layout::is_broadcastable_to(
		make_span(canonical_output_shapes[0]),
		make_span(reference_shape)
	);
	if (!valid)
	{
		throw std::invalid_argument(
			"elementwise_shape_policy requires the user-supplied "
			"output shape to be broadcast-compatible with the inputs."
		);
	}
}

const elementwise_shape_policy&
elementwise_shape_policy::get() noexcept
{
	static const elementwise_shape_policy instance;
	return instance;
}

} // namespace operations
} // namespace xmipp4
