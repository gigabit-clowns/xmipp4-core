// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/policies/reduction_operation_shape_policy.hpp>

#include <xmipp4/core/layout/broadcast.hpp>

#include <algorithm>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace xmipp4
{
namespace ops
{

reduction_operation_shape_policy::reduction_operation_shape_policy(
	axis_list_type axes,
	bool keep_dimensions
)
	: m_axes(std::move(axes))
	, m_keep_dimensions(keep_dimensions)
{
	// Ascending order is what lets the deduction erase the reduced axes
	// back to front without invalidating the indices it has yet to use.
	std::sort(m_axes.begin(), m_axes.end());

	const auto repeated = std::adjacent_find(m_axes.begin(), m_axes.end());
	if (repeated != m_axes.end())
	{
		std::ostringstream oss;
		oss << "reduction_operation_shape_policy: axis " << *repeated
			<< " is reduced more than once.";
		throw std::invalid_argument(oss.str());
	}
}

span<const std::size_t>
reduction_operation_shape_policy::get_axes() const noexcept
{
	return make_span(m_axes.data(), m_axes.size());
}

bool reduction_operation_shape_policy::get_keep_dimensions() const noexcept
{
	return m_keep_dimensions;
}

void reduction_operation_shape_policy::deduce(
	const operation_descriptor &descriptor,
	span<shape_type> canonical_output_shapes,
	span<const shape_type> input_shapes
) const
{
	if (input_shapes.empty())
	{
		std::ostringstream oss;
		oss << descriptor << ": a reduction needs at least one input "
			<< "operand to reduce.";
		throw std::invalid_argument(oss.str());
	}

	shape_type shape = input_shapes[0];
	for (std::size_t i = 1; i < input_shapes.size(); ++i)
	{
		broadcast_extents_accumulate(shape, make_span(input_shapes[i]));
	}

	// The rank is only known here, so this is the first place the axes can
	// be checked against it.
	if (!m_axes.empty() && m_axes.back() >= shape.size())
	{
		std::ostringstream oss;
		oss << descriptor << ": axis " << m_axes.back()
			<< " is reduced, but the broadcast operands only have "
			<< shape.size() << " of them.";
		throw std::out_of_range(oss.str());
	}

	if (m_keep_dimensions)
	{
		for (const auto axis : m_axes)
		{
			shape[axis] = 1;
		}
	}
	else
	{
		// Back to front, so that the indices not yet used stay valid.
		// The constructor sorted them ascending for exactly this.
		for (auto ite = m_axes.rbegin(); ite != m_axes.rend(); ++ite)
		{
			shape.erase(std::next(shape.begin(), *ite));
		}
	}

	if (!canonical_output_shapes.empty())
	{
		std::fill(
			canonical_output_shapes.begin(),
			std::prev(canonical_output_shapes.end()),
			shape
		);
		canonical_output_shapes.back() = std::move(shape);
	}
}

} // namespace ops
} // namespace xmipp4
