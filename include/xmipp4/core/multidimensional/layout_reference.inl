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
inline
std::size_t layout_reference<T>::get_rank() const noexcept
{
    return m_layout ? m_layout->get_rank() : 0UL;
}

template <typename T>
inline
bool layout_reference<T>::get_axis(std::size_t index, 
                                   axis_descriptor &out ) const noexcept
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
inline
std::ptrdiff_t layout_reference<T>::get_offset() const noexcept
{
    return m_layout ? m_layout->get_offset() : 0;
}

template <typename T>
inline
layout_flags layout_reference<T>::get_flags() const noexcept
{
    XMIPP4_CONST_CONSTEXPR auto empty_flags = 
        layout_flag_bits::contiguous |
        layout_flag_bits::column_major |
        layout_flag_bits::row_major ;

    return m_layout ? m_layout->get_flags() : empty_flags;
}



template <typename T>
inline
layout_reference<T> layout_reference<T>::transpose() const
{
    return apply(std::mem_fn(&layout_type::transpose));
}

template <typename T>
inline
layout_reference<T>& layout_reference<T>::transpose_inplace() noexcept
{
    return apply_inplace(std::mem_fn(&layout_type::transpose_inplace));
}

template <typename T>
inline
layout_reference<T> layout_reference<T>::permute(span<std::size_t> order) const
{
    if (!m_layout && !order.empty())
    {
        throw std::invalid_argument("cannot permute empty layout");
    }

    return apply(std::mem_fn(&layout_type::permute, order));
}

template <typename T>
inline
layout_reference<T>& 
layout_reference<T>::permute_inplace(span<std::size_t> order)
{
    if (!m_layout && !order.empty())
    {
        throw std::invalid_argument("cannot permute empty layout");
    }

    return apply_inplace(std::mem_fn(&layout_type::permute_inplace, order));
}

template <typename T>
inline
layout_reference<T> 
layout_reference<T>::swap_axes(std::size_t axis1, std::size_t axis2) const
{
    if (!m_layout)
    {
        throw std::invalid_argument("cannot swap axes on empty layout");
    }

    return apply(std::mem_fn(&layout_type::swap_axes, axis1, axis2));
}

template <typename T>
inline
layout_reference<T>& 
layout_reference<T>::swap_axes_inplace(std::size_t axis1, std::size_t axis2)
{
    if (!m_layout)
    {
        throw std::invalid_argument("cannot swap axes on empty layout");
    }

    return apply_inplace(
        std::mem_fn(&layout_type::swap_axes_inplace, axis1, axis2)
    );
}

template <typename T>
inline
layout_reference<T> layout_reference<T>::squeeze() const
{
    return apply(std::mem_fn(&layout_type::squeeze));
}

template <typename T>
inline
layout_reference<T>& layout_reference<T>::squeeze_inplace() noexcept
{
    return apply_inplace(std::mem_fn(&layout_type::squeeze_inplace));
}

template <typename T>
inline
layout_reference<T> layout_reference<T>::ravel() const
{
    return apply(std::mem_fn(&layout_type::ravel));
}

template <typename T>
inline
layout_reference<T>& layout_reference<T>::ravel_inplace() noexcept
{
    return apply_inplace(std::mem_fn(&layout_type::ravel_inplace));
}



template <typename T>
inline
void layout_reference<T>::copy_on_write()
{
    if (m_layout.use_count() > 1)
    {
        m_layout = std::make_shared<layout_type>(*m_layout);
    }
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
layout_reference<T>& 
layout_reference<T>::apply_inplace(Func &&func, Args&& ...args)
{
    if (m_layout)
    {
        copy_on_write();
        std::forward<Func>(func)(*m_layout, std::forward<Args>(args)...);
    }

    return *this;
}

} // namespace multidimensional
} // namespace xmipp4
