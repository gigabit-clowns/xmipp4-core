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
XMIPP4_NODISCARD inline
const typename array<Storage, Layout>::storage_type& 
array<Storage, Layout>::get_storage() const noexcept
{
    return m_storage;
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
const typename array<Storage, Layout>::layout_type& 
array<Storage, Layout>::get_layout() const noexcept
{
    return m_layout;
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
std::size_t array<Storage, Layout>::get_rank() const noexcept
{
    return m_layout.get_rank();
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::view_type
array<Storage, Layout>::apply_subscripts(span<const dynamic_subscript> subscripts)
{
    return array(m_storage.view(), m_layout.apply_subscripts(subscripts));
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::const_view_type
array<Storage, Layout>::apply_subscripts(span<const dynamic_subscript> subscripts) const
{
    return array(m_storage.view(), m_layout.apply_subscripts(subscripts));
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::view_type
array<Storage, Layout>::transpose()
{
    return array(m_storage.view(), m_layout.transpose());
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::const_view_type
array<Storage, Layout>::transpose() const
{
    return array(m_storage.view(), m_layout.transpose());
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::view_type
array<Storage, Layout>::permute(span<std::size_t> order)
{
    return array(m_storage.view(), m_layout.permute(order));
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::const_view_type
array<Storage, Layout>::permute(span<std::size_t> order) const
{
    return array(m_storage.view(), m_layout.permute(order));
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::view_type
array<Storage, Layout>::swap_axes(std::size_t axis1, std::size_t axis2)
{
    return array(m_storage.view(), m_layout.swap_axes(axis1, axis2));
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::const_view_type
array<Storage, Layout>::swap_axes(std::size_t axis1, std::size_t axis2) const
{
    return array(m_storage.view(), m_layout.swap_axes(axis1, axis2));
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::view_type
array<Storage, Layout>::squeeze()
{
    return array(m_storage.view(), m_layout.squeeze());
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::const_view_type
array<Storage, Layout>::squeeze() const
{
    return array(m_storage.view(), m_layout.squeeze());
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
void array<Storage, Layout>::broadcast_extents_to_layout(std::vector<std::size_t> &extents,
                                                         std::size_t trailing_dimensions ) const
{
    m_layout.broadcast_extents_to_layout(extents, trailing_dimensions);
}

template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::view_type
array<Storage, Layout>::broadcast_layout_to_extents(span<const std::size_t> extents,
                                                    std::size_t trailing_dimensions )
{
    return array(
        m_storage.view(), 
        m_layout.broadcast_layout_to_extents(extents, trailing_dimensions)
    );
}


template <typename Storage, typename Layout>
XMIPP4_NODISCARD inline
typename array<Storage, Layout>::const_view_type
array<Storage, Layout>::broadcast_layout_to_extents(span<const std::size_t> extents,
                                                    std::size_t trailing_dimensions ) const
{
    return array(
        m_storage.view(), 
        m_layout.broadcast_layout_to_extents(extents, trailing_dimensions)
    );
}

} // namespace multidimensional
} // namespace xmipp4
