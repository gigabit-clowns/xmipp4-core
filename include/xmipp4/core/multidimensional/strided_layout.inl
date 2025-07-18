// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "strided_layout.hpp"

#include "checks.hpp"

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout::apply_subscripts_helper
{
public:
    template <typename BidirIt1, typename BidirIt2>
    static
    strided_layout process(BidirIt1 first_subscript, BidirIt1 last_subscript,
                           BidirIt2 first_axis, BidirIt2 last_axis,
                           std::ptrdiff_t offset )
    {
        std::vector<strided_axis> axes; 

        process_forwards(
            first_subscript, last_subscript,
            first_axis, last_axis,
            offset,
            axes, axes.begin()
        );

        return strided_layout(std::move(axes), offset);
    }

private:
    template <typename BidirIt1, typename BidirIt2>
    static
    void process_forwards(BidirIt1 first_subscript, BidirIt1 last_subscript,
                          BidirIt2 first_axis, BidirIt2 last_axis, 
                          std::ptrdiff_t &offset,
                          std::vector<strided_axis> &axes,
                          std::vector<strided_axis>::iterator head_ite )
    {
        while (first_axis != last_axis && first_subscript != last_subscript)
        {
            const auto &subscript = *first_subscript;
            const auto &axis = *first_axis;

            switch (subscript.get_subscript_type())
            {
            case dynamic_subscript::subscript_type::ellipsis:
                process_backwards(
                    std::next(first_subscript), last_subscript,
                    first_axis, last_axis,
                    offset,
                    axes,
                    head_ite
                );
                first_subscript = last_subscript; // Finishes processing
                first_axis = last_axis;
                break;

            case dynamic_subscript::subscript_type::new_axis:
                head_ite = axes.insert(head_ite, make_phantom_axis());
                ++first_subscript;
                ++head_ite;
                break;

            case dynamic_subscript::subscript_type::index:
                apply_index(axis, offset, subscript.get_index());
                ++first_subscript;
                ++first_axis;
                break;
            
            case dynamic_subscript::subscript_type::slice:
                head_ite = axes.insert(head_ite, axis);
                apply_slice(*head_ite, offset, subscript.get_slice());
                ++first_subscript;
                ++first_axis;
                ++head_ite;
                break;

            default:
                throw std::logic_error("Unknown subscript type encountered");
                break;
            }
        }
        
        if (first_subscript != last_subscript)
        {
            throw std::invalid_argument("Not all subscripts were processed");
        }

        // Copy reminder
        if (first_axis != last_axis)
        {
            axes.insert(head_ite, first_axis, last_axis);
        }
    }

    template <typename BidirIt1, typename BidirIt2>
    static
    void process_backwards(BidirIt1 first_subscript, BidirIt1 last_subscript,
                           BidirIt2 first_axis, BidirIt2 last_axis, 
                           std::ptrdiff_t &offset,
                           std::vector<strided_axis> &axes,
                           std::vector<strided_axis>::iterator head_ite )
    {
        while (first_axis != last_axis && first_subscript != last_subscript)
        {
            const auto &subscript = *std::prev(last_subscript);
            const auto &axis = *std::prev(last_axis);

            switch (subscript.get_subscript_type())
            {
            case dynamic_subscript::subscript_type::ellipsis:
                throw std::invalid_argument(
                    "Two ellipsis tags were encountered when processing "
                    "subscripts"
                );
                break;

            case dynamic_subscript::subscript_type::new_axis:
                head_ite = axes.insert(head_ite, make_phantom_axis());
                --last_subscript;
                break;

            case dynamic_subscript::subscript_type::index:
                apply_index(axis, offset, subscript.get_index());
                --last_subscript;
                --last_axis;
                break;
            
            case dynamic_subscript::subscript_type::slice:
                head_ite = axes.insert(head_ite, axis);
                apply_slice(*head_ite, offset, subscript.get_slice());
                --last_subscript;
                --last_axis;
                break;

            default:
                throw std::logic_error("Unknown subscript type encountered");
                break;
            }
        }
        
        if (first_subscript != last_subscript)
        {
            throw std::invalid_argument("Not all subscripts were processed");
        }

        // Copy reminder
        if (first_axis != last_axis)
        {
            axes.insert(head_ite, first_axis, last_axis);
        }
    }

};





inline
strided_layout::strided_layout(std::vector<strided_axis> &&axes,
                               std::ptrdiff_t offset ) noexcept
    : m_axes(std::move(axes))
    , m_offset(offset)
{
}

inline
strided_layout::strided_layout(const std::size_t *extents, 
                               std::size_t rank,
                               std::ptrdiff_t offset )
    : m_offset(offset)
{
    m_axes.reserve(rank);
    for (std::size_t i = 0; i < rank; ++i)
    {
        m_axes.emplace_back(extents[i], 0);
    }

    std::size_t stride = 1;
    for(auto ite = m_axes.rbegin(); ite != m_axes.rend(); ++ite)
    {
        ite->set_stride(stride);
        stride *= ite->get_extent();
    }
}

inline
strided_layout::strided_layout(const std::size_t *extents, 
                               const std::ptrdiff_t *strides, 
                               std::size_t rank,
                               std::ptrdiff_t offset )
    : m_offset(offset)
{
    m_axes.reserve(rank);
    for (std::size_t i = 0; i < rank; ++i)
    {
        m_axes.emplace_back(extents[i], strides[i]);
    }
}

inline
strided_layout::strided_layout(const strided_axis *axes, 
                               std::size_t rank,
                               std::ptrdiff_t offset )
    : m_axes(axes, axes + rank)
    , m_offset(offset)
{
}

XMIPP4_NODISCARD inline
std::size_t strided_layout::get_rank() const noexcept
{
    return m_axes.size();
}

inline
std::size_t strided_layout::get_axes(std::size_t *extents, 
                                     std::ptrdiff_t *strides,
                                     std::size_t count ) const noexcept
{
    if (count > m_axes.size())
    {
        count = m_axes.size();
    }

    for (std::size_t i = 0; i < count; ++i)
    {
        const auto &axis = m_axes[i];
        if(extents)
        {
            extents[i] = axis.get_extent();
        }
        if(strides)
        {
            strides[i] = axis.get_stride();
        }
    }

    return count;
}

inline
bool strided_layout::get_axis(std::size_t index, 
                              strided_axis &out ) const noexcept
{
    const bool result = (index < m_axes.size());
    if (result)
    {
        out = m_axes[index];
    }
    return result;
}

XMIPP4_NODISCARD inline
std::ptrdiff_t strided_layout::get_offset() const noexcept
{
    return m_offset;
}

XMIPP4_NODISCARD inline
std::size_t strided_layout::compute_storage_requirement() const noexcept
{
    std::size_t result = 0;

    bool zero_extent = false;
    for (const auto &axis : m_axes)
    {
        const auto extent = axis.get_extent();
        if (extent == 0)
        {
            zero_extent = true;
            break;
        }

        const auto stride = axis.get_stride();
        if(stride > 0)
        {
            const std::size_t last_index = extent - 1;
            result += last_index*stride;
        }
    }

    if (zero_extent)
    {
        result = 0;
    }
    else
    {
        ++result;
        result += m_offset;
    }

    return result;
}


XMIPP4_NODISCARD inline
strided_layout 
strided_layout::apply_subscripts(span<const dynamic_subscript> subscripts) const
{
    return apply_subscripts_helper::process(
        subscripts.begin(), subscripts.end(),
        m_axes.cbegin(), m_axes.cend(),
        m_offset
    );
}

XMIPP4_NODISCARD inline
strided_layout strided_layout::transpose() const
{
    std::vector<strided_axis> axes;
    axes.reserve(m_axes.size());

    std::reverse_copy(
        m_axes.cbegin(), m_axes.cend(),
        std::back_inserter(axes)
    );

    return strided_layout(std::move(axes), m_offset);
}

XMIPP4_NODISCARD inline
strided_layout strided_layout::permute(span<const std::size_t> order) const
{
    const auto count = m_axes.size();
    check_axis_permutation(order.begin(), order.end(), count);

    std::vector<strided_axis> axes;
    axes.reserve(count);

    for(std::size_t i = 0; i < count; ++i)
    {
        axes.push_back(m_axes[order[i]]);
    }

    return strided_layout(std::move(axes), m_offset);
}

XMIPP4_NODISCARD inline
strided_layout 
strided_layout::swap_axes(std::size_t axis1, std::size_t axis2) const
{
    if (axis1 >= m_axes.size())
    {
        throw std::out_of_range("axis1 exceeds bounds");
    }
    if (axis2 >= m_axes.size())
    {
        throw std::out_of_range("axis2 exceeds bounds");
    }

    auto axes = m_axes;
    std::swap(axes[axis1], axes[axis2]);

    return strided_layout(std::move(axes), m_offset);
}

XMIPP4_NODISCARD inline
strided_layout strided_layout::squeeze() const
{
    std::vector<strided_axis> axes;

    std::copy_if(
        m_axes.cbegin(), m_axes.cend(),
        std::back_inserter(axes),
        is_significant
    );

    return strided_layout(std::move(axes), m_offset);
}

inline
void 
strided_layout::broadcast_extents_to_layout(std::vector<std::size_t> &extents,
                                            std::size_t trailing_dimensions ) const
{
    if (trailing_dimensions > m_axes.size())
    {
        throw std::invalid_argument(
            "Requesting more trailing dimensions than axes in the layout"
        );
    }

    const std::size_t count = m_axes.size() - trailing_dimensions;
    if (count < extents.size())
    {
        std::ostringstream oss;
        oss << "Can not broadcast shape with " << extents.size()
            << " axes into a layout of " << m_axes.size() 
            << " axes with " << trailing_dimensions 
            << " trailing dimensions";
        throw std::invalid_argument(oss.str());
    }
    
    if (count > extents.size())
    {
        const std::size_t old_size = extents.size();
        const std::size_t new_size = count;
        const std::size_t padding = new_size - old_size;
        extents.insert(
            extents.cbegin(),
            padding,
            1UL
        );
    }

    for(std::size_t i = 0; i < count; ++i)
    {
        if (!multidimensional::broadcast_extent_to_axis(extents[i], m_axes[i]))
        {
            std::ostringstream oss;
            oss << "Can not broadcast extent " << extents[i]
                << " into an axis of extent " << m_axes[i].get_extent();
            throw std::invalid_argument(oss.str());
        }
    }
}

XMIPP4_NODISCARD inline
strided_layout 
strided_layout::broadcast_layout_to_extents(span<const std::size_t> extents,
                             std::size_t trailing_dimensions ) const
{
    const auto total = extents.size() + trailing_dimensions;
    if (m_axes.size() > total)
    {
        std::ostringstream oss;
        oss << "Can not broadcast layout with " << m_axes.size()
            << " axes into a shape of " << extents.size() 
            << " axes and " << trailing_dimensions
            << " trailing dimensions.";
        throw std::invalid_argument(oss.str());
    }

    std::vector<strided_axis> axes;
    axes.reserve(total);
    
    const std::size_t padding = total - m_axes.size();
    std::fill_n(
        std::back_inserter(axes), padding,
        make_phantom_axis()
    );
    std::copy(
        m_axes.cbegin(), m_axes.cend(),
        std::back_inserter(axes)
    );

    const auto count = extents.size();
    for(std::size_t i = 0; i < count; ++i)
    {
        if (!multidimensional::broadcast_axis_to_extent(axes[i], extents[i]))
        {
            std::ostringstream oss;
            oss << "Can not broadcast axis of extent " << axes[i].get_extent()
                << " into an extent of " << extents[i] << ".";
            throw std::invalid_argument(oss.str());
        }
    }

    return strided_layout(std::move(axes), m_offset);
}

} // namespace multidimensional
} // namespace xmipp4
