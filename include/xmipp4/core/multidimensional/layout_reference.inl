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
inline
layout_reference<T>::layout_reference(const std::size_t *extents, 
                                      const std::ptrdiff_t *strides, 
                                      std::size_t rank,
                                      std::ptrdiff_t offset )
    : m_layout(std::make_shared<layout_type>(extents, strides, rank, offset))
{
}

template <typename T>
inline
layout_reference<T>::layout_reference(const axis_descriptor *axes, 
                                      std::size_t rank,
                                      std::ptrdiff_t offset )
    : m_layout(std::make_shared<layout_type>(axes, rank, offset))
{
}

template <typename T>
inline
layout_reference<T>::layout_reference(const layout_type &other)
    : m_layout(std::make_shared<layout_type>(other))
{
}

template <typename T>
inline
layout_reference<T>::layout_reference(layout_type &&other)
    : m_layout(std::make_shared<layout_type>(std::move(other)))
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
layout_reference<T> layout_reference<T>::transpose() const
{
    layout_reference result;

    if (m_layout)
    {
        result = layout_reference(m_layout->transpose());
    }

    return result;
}

template <typename T>
inline
layout_reference<T>& layout_reference<T>::transpose_inplace() noexcept
{
    if (m_layout)
    {
        copy_on_write();
        m_layout->transpose_inplace();
    }

    return *this;
}

template <typename T>
inline
layout_reference<T> layout_reference<T>::matrix_transpose() const
{
    layout_reference result;

    if (m_layout)
    {
        result = layout_reference(m_layout->matrix_transpose());
    }

    return result;
}

template <typename T>
inline
layout_reference<T>& layout_reference<T>::matrix_transpose_inplace() noexcept
{
    if (m_layout)
    {
        copy_on_write();
        m_layout->matrix_transpose_inplace();
    }

    return *this;
}

template <typename T>
inline
layout_reference<T> layout_reference<T>::squeeze() const
{
    layout_reference result;

    if (m_layout)
    {
        result = layout_reference(m_layout->squeeze());
    }

    return result;
}

template <typename T>
inline
layout_reference<T>& layout_reference<T>::squeeze_inplace() noexcept
{
    copy_on_write();
    m_layout->squeeze_inplace();
    return *this;
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

} // namespace multidimensional
} // namespace xmipp4
