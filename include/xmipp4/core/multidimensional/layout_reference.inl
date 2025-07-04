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
 * @file layout_reference.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of layout_reference.hpp
 * @date 2025-02-03
 * 
 */

#include "layout_reference.hpp"

#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
template <typename... Args>
inline
layout_reference<T>::layout_reference(Args&& ...args)
    : m_layout(std::make_shared<layout_type>(std::forward<Args>(args)...))
{

}



template <typename T>
XMIPP4_NODISCARD inline
std::size_t layout_reference<T>::get_rank() const noexcept
{
    return m_layout ? m_layout->get_rank() : 0UL;
}

template <typename T>
inline
bool layout_reference<T>::get_axis(std::size_t index, 
                                   strided_axis &out ) const noexcept
{
    bool result;

    if (m_layout)
    {
        result = m_layout->get_axis(index, out);
    }
    else
    {
        result = false;
    }

    return result;
}

template <typename T>
XMIPP4_NODISCARD inline
std::ptrdiff_t layout_reference<T>::get_offset() const noexcept
{
    return m_layout ? m_layout->get_offset() : 0;
}

template <typename T>
XMIPP4_NODISCARD inline
std::size_t layout_reference<T>::compute_storage_requirement() const noexcept
{
    return m_layout ? m_layout->compute_storage_requirement() : 0;
}

template <typename T>
XMIPP4_NODISCARD inline
std::shared_ptr<const typename layout_reference<T>::layout_type> 
layout_reference<T>::get_shared() const noexcept
{
    return m_layout;
}



template <typename T>
XMIPP4_NODISCARD inline
layout_reference<T> 
layout_reference<T>::apply_subscripts(span<const dynamic_subscript> subscripts) const
{
    return apply_no_empty(std::mem_fn(&layout_type::apply_subscripts, subscripts));
}

template <typename T>
XMIPP4_NODISCARD inline
layout_reference<T> layout_reference<T>::transpose() const
{
    return apply(std::mem_fn(&layout_type::transpose));
}

template <typename T>
XMIPP4_NODISCARD inline
layout_reference<T> 
layout_reference<T>::permute(span<const std::size_t> order) const
{
    return apply_no_empty(std::mem_fn(&layout_type::permute, order));
}

template <typename T>
XMIPP4_NODISCARD inline
layout_reference<T> 
layout_reference<T>::swap_axes(std::size_t axis1, std::size_t axis2) const
{
    return apply_no_empty(std::mem_fn(&layout_type::swap_axes, axis1, axis2));
}

template <typename T>
XMIPP4_NODISCARD inline
layout_reference<T> layout_reference<T>::squeeze() const
{
    return apply(std::mem_fn(&layout_type::squeeze));
}

template <typename T>
inline
void layout_reference<T>::broadcast_dry(std::vector<std::size_t> &extents) const
{
    if (m_layout)
    {
        m_layout->broadcast_dry(extents);
    }
    else if (!extents.empty())
    {
        std::ostringstream oss;
        oss << "Cannot broadcast shape of " << extents.size()
            << " axes into an empty layout";
        throw std::invalid_argument(oss.str());
    }
}

template <typename T>
XMIPP4_NODISCARD inline
layout_reference<T> 
layout_reference<T>::broadcast_to(span<const std::size_t> extents) const
{
    return apply_no_empty(std::mem_fn(&layout_type::broadcast_to, extents));
}



template <typename T>
template <typename Func, typename... Args>
inline
layout_reference<T> layout_reference<T>::apply(Func &&func, Args&& ...args)
{
    layout_reference result;

    if (m_layout)
    {
        result = layout_reference(
            std::forward<Func>(func)(*m_layout, std::forward<Args>(args)...)
        );
    }

    return result;
}

template <typename T>
template <typename Func, typename... Args>
inline
layout_reference<T> layout_reference<T>::apply_no_empty(Func &&func, Args&& ...args)
{
    layout_reference result;

    if (m_layout)
    {
        result = layout_reference(
            std::forward<Func>(func)(*m_layout, std::forward<Args>(args)...)
        );
    }
    else
    {
        // Apply it on a default constructed layout
        layout_type layout;
        result = layout_reference(
            std::forward<Func>(func)(layout, std::forward<Args>(args)...)
        );
    }

    return result;
}

} // namespace multidimensional
} // namespace xmipp4
