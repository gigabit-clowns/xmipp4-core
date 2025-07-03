#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file strided_layout.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of strided_layout.hpp
 * @date 2025-02-03
 * 
 */

#include "strided_layout.hpp"

#include "checks.hpp"

#include <list>
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
        // Using list instead of vector due to the chaotic insertion pattern
        std::list<strided_axis> axes; 

        process_forwards(
            first_subscript, last_subscript,
            first_axis, last_axis,
            offset,
            axes, axes.begin()
        );

        return strided_layout(
            std::vector<strided_axis>(axes.cbegin(), axes.cend()), 
            offset
        );
    }

private:
    template <typename BidirIt1, typename BidirIt2>
    static
    void process_forwards(BidirIt1 first_subscript, BidirIt1 last_subscript,
                          BidirIt2 first_axis, BidirIt2 last_axis, 
                          std::ptrdiff_t &offset,
                          std::list<strided_axis> &axes,
                          std::list<strided_axis>::iterator head_ite )
    {
        while (first_axis != last_axis && first_subscript != last_subscript)
        {
            const auto &subscript = *first_subscript;
            const auto &axis = *first_axis;

            switch (first_subscript->get_subscript_type())
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
                apply_index_safe(axis, offset, subscript.get_index());
                ++first_subscript;
                ++first_axis;
                break;
            
            case dynamic_subscript::subscript_type::slice:
                head_ite = axes.insert(head_ite, axis);
                apply_slice_safe(*head_ite, offset, subscript.get_slice());
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
        axes.insert(head_ite, first_axis, last_axis);
    }

    template <typename BidirIt1, typename BidirIt2>
    static
    void process_backwards(BidirIt1 first_subscript, BidirIt1 last_subscript,
                           BidirIt2 first_axis, BidirIt2 last_axis, 
                           std::ptrdiff_t &offset,
                           std::list<strided_axis> &axes,
                           std::list<strided_axis>::iterator head_ite )
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
                apply_index_safe(axis, offset, subscript.get_index());
                --last_subscript;
                --last_axis;
                break;
            
            case dynamic_subscript::subscript_type::slice:
                head_ite = axes.insert(head_ite, axis);
                apply_slice_safe(*head_ite, offset, subscript.get_slice());
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
        axes.insert(head_ite, first_axis, last_axis);
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
void strided_layout::broadcast_dry(std::vector<std::size_t> &extents) const
{
    if (m_axes.size() < extents.size())
    {
        std::ostringstream oss;
        oss << "Can not broadcast shape with " << extents.size()
            << " axes into a layout of " << m_axes.size() << " axes.";
        throw std::invalid_argument(oss.str());
    }
    
    if (m_axes.size() > extents.size())
    {
        const std::size_t old_size = extents.size();
        const std::size_t new_size = m_axes.size();
        const std::size_t padding = new_size - old_size;
        extents.insert(
            extents.cbegin(),
            padding,
            1UL
        );
    }

    const std::size_t count = extents.size(); 
    for(std::size_t i = 0; i < count; ++i)
    {
        if (!multidimensional::broadcast_dry(m_axes[i], extents[i]))
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
strided_layout::broadcast_to(span<const std::size_t> extents) const
{

    if (m_axes.size() > extents.size())
    {
        std::ostringstream oss;
        oss << "Can not broadcast layout with " << m_axes.size()
            << " axes into a shape of " << extents.size() << " axes.";
        throw std::invalid_argument(oss.str());
    }

    std::vector<strided_axis> axes;
    axes.reserve(extents.size());
    
    const auto padding = extents.size() - m_axes.size();
    std::fill_n(
        std::back_inserter(axes), padding,
        make_phantom_axis()
    );
    std::copy(
        m_axes.cbegin(), m_axes.cend(),
        std::back_inserter(axes)
    );

    const std::size_t count = extents.size(); 
    for(std::size_t i = 0; i < count; ++i)
    {
        if (!multidimensional::broadcast_to(axes[i], extents[i]))
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
