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

#include "strided_layout_utils.hpp"
#include "checks.hpp"

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

    compute_contiguous_axis_strides(
        m_axes.rbegin(), 
        m_axes.rend()
    );
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
dynamic_layout::dynamic_layout(const axis_descriptor *axes, 
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
                              axis_descriptor &out ) const noexcept
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
dynamic_layout dynamic_layout::apply_subscripts(span<dynamic_subscript> subscripts) const
{
    dynamic_layout result = *this;
    result.apply_subscripts_inplace(subscripts);
    return result;
}

inline
dynamic_layout& dynamic_layout::apply_subscripts_inplace(span<dynamic_subscript> subscripts)
{
    auto axis_ite = m_axes.begin();
    auto subscript_ite = subscripts.begin();

    while (axis_ite != m_axes.end() && subscript_ite != subscripts.end())
    {
        switch (subscript_ite->get_subscript_type())
        {
        case dynamic_subscript::subscript_type::ellipsis:
            throw std::runtime_error("Not implemented"); // TODO
            break;

        case dynamic_subscript::subscript_type::new_axis:
            axis_ite = m_axes.insert(axis_ite, make_phantom_axis());
            ++axis_ite;
            ++subscript_ite;
            break;

        case dynamic_subscript::subscript_type::index:
            apply_index(*axis_ite, subscript_ite->get_index(), m_offset);
            axis_ite = m_axes.erase(axis_ite);
            ++subscript_ite;
            break;

        case dynamic_subscript::subscript_type::slice:
            apply_slice(*axis_ite, subscript_ite->get_slice(), m_offset);
            ++axis_ite;
            ++subscript_ite;
            break;
        }
    }
    
    if (subscript_ite != subscripts.end())
    {
        throw std::invalid_argument("not all subscripts were processed");
    }
    
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
    transpose_layout_inplace(m_axes.begin(), m_axes.end());
    return *this;
}

XMIPP4_NODISCARD inline
dynamic_layout dynamic_layout::permute(span<std::size_t> order) const
{
    dynamic_layout result = *this;
    result.permute_inplace(order);
    return result;
}

inline
dynamic_layout& dynamic_layout::permute_inplace(span<std::size_t> order)
{
    check_axis_permutation(order.begin(), order.end(), m_axes.size());

    std::vector<axis_descriptor> tmp;
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
    m_axes.erase(
        squeeze_layout_inplace(m_axes.begin(), m_axes.end()), 
        m_axes.end()
    );
    return *this;
}

XMIPP4_NODISCARD inline
dynamic_layout dynamic_layout::ravel() const
{
    dynamic_layout result = *this;
    result.ravel_inplace();
    return result;
}

inline
dynamic_layout& dynamic_layout::ravel_inplace() noexcept
{
    sort_layout_inplace(m_axes.begin(), m_axes.end());
    const auto ite = ravel_layout_inplace(
        m_axes.begin(), 
        m_axes.end(), 
        m_offset
    );
    m_axes.erase(ite, m_axes.end());

    return *this;
}

} // namespace multidimensional
} // namespace xmipp4
