// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_shape_policies/reduction_shape_policy.hpp>

#include <xmipp4/core/multidimensional/broadcast.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

namespace
{

std::vector<std::size_t>
sort_and_deduplicate(std::vector<std::size_t> axes)
{
    std::sort(axes.begin(), axes.end());
    axes.erase(std::unique(axes.begin(), axes.end()), axes.end());
    return axes;
}

void remove_at_indices(
	std::vector<std::size_t>& data, 
	const std::vector<std::size_t>& indices
)
{
	XMIPP4_ASSERT( std::is_sorted(indices.cbegin(), indices.cend()) );
    for (auto it = indices.rbegin(); it != indices.rend(); ++it) 
	{
		XMIPP4_ASSERT( *it < data.size() );
        data.erase(std::next(data.begin(), *it));
    }
}

void write_ones_at_indices(
	std::vector<std::size_t>& data, 
	const std::vector<std::size_t>& indices
)
{
	for (const auto index : indices)
	{
		XMIPP4_ASSERT( index < data.size() );
		data[index] = 1;
	}
}

} // anonymous namespace

reduction_operation_shape_policy::reduction_operation_shape_policy(
    const std::vector<std::size_t>& axes,
    bool keep_dim
)
    : m_reduction_axes(sort_and_deduplicate(axes))
    , m_keep_dimensions(keep_dim)
{
}

reduction_operation_shape_policy::reduction_operation_shape_policy(
    std::vector<std::size_t>&& axes,
    bool keep_dim
)
    : m_reduction_axes(sort_and_deduplicate(std::move(axes)))
    , m_keep_dimensions(keep_dim)
{
}

reduction_operation_shape_policy::~reduction_operation_shape_policy() = default;

span<const std::size_t>
reduction_operation_shape_policy::get_reduction_axes() const noexcept
{
    return make_span(m_reduction_axes);
}

bool reduction_operation_shape_policy::keeps_dimensions() const noexcept
{
    return m_keep_dimensions;
}

void reduction_operation_shape_policy::deduce(
    span<shape_type> canonical_output_shapes,
    span<const shape_type> input_shapes
) const
{
    if (input_shapes.empty())
    {
        throw std::invalid_argument(
            "reduction_operation_shape_policy requires at least one input."
        );
    }

    // Broadcast all inputs to obtain the reduction space.
    shape_type reduction_space = input_shapes[0];
    for (std::size_t i = 1; i < input_shapes.size(); ++i)
    {
        broadcast_extents_accumulate(
            reduction_space,
            make_span(input_shapes[i])
        );
    }

    const auto rank = reduction_space.size();
	for (const auto axis : m_reduction_axes)
	{
		if (axis >= rank)
		{
			std::ostringstream oss;
			oss << "reduction_operation_shape_policy: axis "
				<< axis
				<< " is out of range for input with rank "
				<< rank;
			throw std::invalid_argument(oss.str());
		}
	}

    // Empty axes means reduce all dimensions.
    const bool reduce_all = m_reduction_axes.empty();
    shape_type output_shape;
	if (!reduce_all)
	{
		output_shape = std::move(reduction_space); // Not longer needed.
		if (m_keep_dimensions)
		{
			write_ones_at_indices(output_shape, m_reduction_axes);
		}
		else
		{
			remove_at_indices(output_shape, m_reduction_axes);
		}
	}
	else if (m_keep_dimensions)
	{
		output_shape = shape_type(rank, 1UL);
	}

    std::fill(
        canonical_output_shapes.begin(),
        canonical_output_shapes.end(),
        output_shape
    );
}

} // namespace multidimensional
} // namespace xmipp4
