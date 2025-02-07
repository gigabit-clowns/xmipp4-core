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
 * @file array.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation array.hpp
 * @date 2025-02-07
 * 
 */

#include "array.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename Storage, typename Layout>
inline
array<Storage, Layout>::array(storage_type storage, layout_type layout)
    : m_storage(std::move(storage))
    , m_layout(std::move(layout))
{
}



template <typename Storage, typename Layout>
inline
const typename array<Storage, Layout>::storage_type& 
array<Storage, Layout>::get_storage() const noexcept
{
    return m_storage;
}

template <typename Storage, typename Layout>
inline
const typename array<Storage, Layout>::layout_type& 
array<Storage, Layout>::get_layout() const noexcept
{
    return m_layout;
}

template <typename Storage, typename Layout>
inline
std::size_t array<Storage, Layout>::get_rank() const noexcept
{
    return m_layout.get_rank();
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout> 
array<Storage, Layout>::transpose() const
{
    return array(m_storage, m_layout.transpose());
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout>& 
array<Storage, Layout>::transpose_inplace() noexcept
{
    m_layout.transpose_inplace();
    return *this;
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout> 
array<Storage, Layout>::permute(span<std::size_t> order) const
{
    return array(m_storage, m_layout.permute(order));
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout>& 
array<Storage, Layout>::permute_inplace(span<std::size_t> order)
{
    m_layout.permute_inplace(order);
    return *this;
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout> 
array<Storage, Layout>::swap_axes(std::size_t axis1, std::size_t axis2) const
{
    return array(m_storage, m_layout.swap_axes(axis1, axis2));
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout>& 
array<Storage, Layout>::swap_axes_inplace(std::size_t axis1, std::size_t axis2)
{
    m_layout.swap_axes_inplace(axis1, axis2);
    return *this;
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout> 
array<Storage, Layout>::squeeze() const
{
    return array(m_storage, m_layout.squeeze());
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout>& 
array<Storage, Layout>::squeeze_inplace() noexcept
{
    m_layout.squeeze_inplace();
    return *this;
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout> 
array<Storage, Layout>::ravel() const
{
    return array(m_storage, m_layout.ravel());
}

template <typename Storage, typename Layout>
inline
array<Storage, Layout>& 
array<Storage, Layout>::ravel_inplace() noexcept
{
    m_layout.ravel_inplace();
    return *this;
}

} // namespace multidimensional
} // namespace xmipp4
