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
 * @file dynamic_layout.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of dynamic_layout.hpp
 * @date 2025-02-03
 * 
 */

#include "dynamic_layout.hpp"

#include "checks.hpp"

#include <algorithm>

namespace xmipp4 
{
namespace multidimensional
{

inline
dynamic_layout::dynamic_layout(const std::size_t *extents, 
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
dynamic_layout::dynamic_layout(const std::size_t *extents, 
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
dynamic_layout::dynamic_layout(const strided_axis *axes, 
                               std::size_t rank,
                               std::ptrdiff_t offset )
    : m_axes(axes, axes + rank)
    , m_offset(offset)
{
}

XMIPP4_NODISCARD inline
std::size_t dynamic_layout::get_rank() const noexcept
{
    return m_axes.size();
}

inline
std::size_t dynamic_layout::get_axes(std::size_t *extents, 
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
bool dynamic_layout::get_axis(std::size_t index, 
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
std::ptrdiff_t dynamic_layout::get_offset() const noexcept
{
    return m_offset;
}



XMIPP4_NODISCARD inline
dynamic_layout dynamic_layout::apply_subscripts(span<const dynamic_subscript> subscripts) const
{
    dynamic_layout result = *this;
    result.apply_subscripts_inplace(subscripts);
    return result;
}

inline
dynamic_layout& dynamic_layout::apply_subscripts_inplace(span<const dynamic_subscript> subscripts)
{
    apply_subscripts_helper(
        m_axes.begin(), m_axes.end(),
        subscripts.begin(), subscripts.end()
    );
    
    return *this;
}

XMIPP4_NODISCARD inline
dynamic_layout dynamic_layout::transpose() const
{
    dynamic_layout result = *this;
    result.transpose_inplace();
    return result;
}

inline
dynamic_layout& dynamic_layout::transpose_inplace() noexcept
{
    std::reverse(m_axes.begin(), m_axes.end());
    return *this;
}

XMIPP4_NODISCARD inline
dynamic_layout dynamic_layout::permute(span<const std::size_t> order) const
{
    dynamic_layout result = *this;
    result.permute_inplace(order);
    return result;
}

inline
dynamic_layout& dynamic_layout::permute_inplace(span<const std::size_t> order)
{
    check_axis_permutation(order.begin(), order.end(), m_axes.size());

    std::vector<strided_axis> tmp;
    tmp.reserve(m_axes.size());
    for(std::size_t i = 0; i < order.size(); ++i)
    {
        tmp.push_back(m_axes[order[i]]);
    }

    m_axes = std::move(tmp);
    return *this;
}

XMIPP4_NODISCARD inline
dynamic_layout 
dynamic_layout::swap_axes(std::size_t axis1, std::size_t axis2) const
{
    dynamic_layout result = *this;
    result.swap_axes_inplace(axis1, axis2);
    return result;
}

inline
dynamic_layout& 
dynamic_layout::swap_axes_inplace(std::size_t axis1, std::size_t axis2)
{
    if (axis1 >= m_axes.size())
    {
        throw std::invalid_argument("axis1 exceeds bounds");
    }
    if (axis2 >= m_axes.size())
    {
        throw std::invalid_argument("axis2 exceeds bounds");
    }

    std::swap(m_axes[axis1], m_axes[axis2]);
    return *this;
}

XMIPP4_NODISCARD inline
dynamic_layout dynamic_layout::squeeze() const
{
    dynamic_layout result = *this;
    result.squeeze_inplace();
    return result;
}

inline
dynamic_layout& dynamic_layout::squeeze_inplace() noexcept
{
    const auto last = std::remove_if(
        m_axes.begin(), 
        m_axes.end(), 
        check_squeeze
    );

    m_axes.erase(last, m_axes.end());
    return *this;
}

inline
dynamic_layout dynamic_layout::broadcast(std::vector<std::size_t> &extents) const
{
    dynamic_layout result = *this;
    result.broadcast_inplace(extents);
    return result;
}

inline
dynamic_layout& dynamic_layout::broadcast_inplace(std::vector<std::size_t> &extents)
{
    if (m_axes.size() < extents.size())
    {
        const std::size_t old_size = m_axes.size();
        const std::size_t new_size = extents.size();
        const std::size_t padding = new_size - old_size;
        m_axes.insert(
            m_axes.cbegin(),
            padding,
            make_phantom_axis()
        );
    }
    else if (m_axes.size() > extents.size())
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

    /*
    const std::size_t count = extents.size(); 
    for(std::size_t i = 0; i < count; ++i)
    {
        if (!multidimensional::broadcast(m_axes[i], extents[i]))
        {
            std::ostringstream ss;
            ss << "axis of extent " << i << "and extent " << extents[i]
               << " cannot be broadcasted.";
            throw std::invalid_argument(ss.str());
        }
    }
    */
}

inline
dynamic_layout dynamic_layout::broadcast_to(span<const std::size_t> extents) const
{
    dynamic_layout result = *this;
    result.broadcast_to_inplace(extents);
    return result;
}

inline
dynamic_layout& dynamic_layout::broadcast_to_inplace(span<const std::size_t> extents)
{
    if (m_axes.size() < extents.size())
    {
        const std::size_t old_size = m_axes.size();
        const std::size_t new_size = extents.size();
        const std::size_t padding = new_size - old_size;
        m_axes.insert(
            m_axes.cbegin(),
            padding,
            make_phantom_axis()
        );
    }
    else if (m_axes.size() > extents.size())
    {
        throw std::invalid_argument("layout has more axes than extents");
    }

    const std::size_t count = extents.size(); 
    for(std::size_t i = 0; i < count; ++i)
    {
        if (!multidimensional::broadcast_to(m_axes[i], extents[i]))
        {
            std::ostringstream ss;
            ss << "axis of extent " << i 
               << " cannot be broadcasted to extent " << extents[i];
            throw std::invalid_argument(ss.str());
        }
    }

    return *this;
}



template <typename Ite1, typename Ite2>
void dynamic_layout::apply_subscripts_helper(Ite1 first_axis, Ite1 last_axis, 
                                             Ite2 first_subscript, Ite2 last_subscript )
{
    while (first_axis != last_axis && first_subscript != last_subscript)
    {
        visit(
            [this, &first_axis, &first_subscript, last_subscript] (auto subscript) -> void
            {
                apply_subscript_helper(
                    first_axis, 
                    first_subscript, 
                    last_subscript, 
                    subscript
                );
            },
            *first_subscript
        );
    }
    
    if (first_subscript != last_subscript)
    {
        throw std::invalid_argument("Not all subscripts were processed");
    }
}

inline
void dynamic_layout::apply_subscript_helper(std::vector<strided_axis>::iterator &axis_ite, 
                                            span<const dynamic_subscript>::iterator &subscript_ite, 
                                            span<const dynamic_subscript>::iterator last_subscript, 
                                            ellipsis_tag )
{
    apply_subscripts_helper(
        std::make_reverse_iterator(axis_ite), 
        m_axes.rend(),
        std::make_reverse_iterator(std::next(subscript_ite)), // Leave this out.
        std::make_reverse_iterator(last_subscript)
    );
    subscript_ite = last_subscript; // Finishes processing.
}

inline 
void dynamic_layout::apply_subscript_helper(std::vector<strided_axis>::iterator &axis_ite, 
                                            span<const dynamic_subscript>::iterator &subscript_ite, 
                                            span<const dynamic_subscript>::iterator, 
                                            new_axis_tag )
{
    axis_ite = m_axes.insert(axis_ite, make_phantom_axis());
    ++axis_ite;
    ++subscript_ite;
}

inline
void dynamic_layout::apply_subscript_helper(std::vector<strided_axis>::iterator &axis_ite,
                                            span<const dynamic_subscript>::iterator &subscript_ite, 
                                            span<const dynamic_subscript>::iterator, 
                                            std::ptrdiff_t index)
{

    const auto sanitized = sanitize_index(index, axis_ite->get_extent());
    apply_index(*axis_ite, m_offset, sanitized);
    axis_ite = m_axes.erase(axis_ite);
    ++subscript_ite;
}

inline
void dynamic_layout::apply_subscript_helper(std::vector<strided_axis>::iterator &axis_ite,
                                            span<const dynamic_subscript>::iterator &subscript_ite, 
                                            span<const dynamic_subscript>::iterator, 
                                            const dynamic_slice &slice)
{
    const auto sanitized_slice = sanitize_slice(slice, axis_ite->get_extent());
    apply_slice(*axis_ite, m_offset, sanitized_slice);
    ++axis_ite;
    ++subscript_ite;
}

inline
void dynamic_layout::apply_subscript_helper(std::vector<strided_axis>::reverse_iterator&, 
                                            span<const dynamic_subscript>::reverse_iterator&, 
                                            span<const dynamic_subscript>::reverse_iterator, 
                                            ellipsis_tag )
{
    throw std::invalid_argument("Two ellipsis tags were encountered");
}

inline
void dynamic_layout::apply_subscript_helper(std::vector<strided_axis>::reverse_iterator &axis_ite, 
                                            span<const dynamic_subscript>::reverse_iterator &subscript_ite, 
                                            span<const dynamic_subscript>::reverse_iterator, 
                                            new_axis_tag )
{
    auto base = axis_ite.base();
    base = m_axes.insert(base, make_phantom_axis());
    axis_ite = std::make_reverse_iterator(base);
    ++subscript_ite;
}

inline
void dynamic_layout::apply_subscript_helper(std::vector<strided_axis>::reverse_iterator &axis_ite,
                                            span<const dynamic_subscript>::reverse_iterator &subscript_ite, 
                                            span<const dynamic_subscript>::reverse_iterator, 
                                            std::ptrdiff_t index)
{
    const auto sanitized = sanitize_index(index, axis_ite->get_extent());
    apply_index(*axis_ite, m_offset, sanitized);
    
    auto base = std::prev(axis_ite.base());
    base = m_axes.erase(base);
    axis_ite = std::make_reverse_iterator(base);
    ++subscript_ite;
}

inline
void dynamic_layout::apply_subscript_helper(std::vector<strided_axis>::reverse_iterator &axis_ite,
                                            span<const dynamic_subscript>::reverse_iterator &subscript_ite, 
                                            span<const dynamic_subscript>::reverse_iterator, 
                                            const dynamic_slice &slice)
{
    const auto sanitized_slice = sanitize_slice(slice, axis_ite->get_extent());
    apply_slice(*axis_ite, m_offset, sanitized_slice);
    ++axis_ite;
    ++subscript_ite;
}

} // namespace multidimensional
} // namespace xmipp4
