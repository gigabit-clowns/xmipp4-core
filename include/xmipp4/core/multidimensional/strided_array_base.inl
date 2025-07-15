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
 * @file strided_array_base.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation strided_array_base.hpp
 * @date 2025-07-15
 * 
 */

#include "strided_array_base.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::view_type
strided_array_base<T>::apply_subscripts(span<const dynamic_subscript> subscripts)
{
    return self_type(
        view_storage(), 
        get_layout().apply_subscripts(subscripts)
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::const_view_type
strided_array_base<T>::apply_subscripts(span<const dynamic_subscript> subscripts) const
{
    return self_type(
        view_storage(), 
        get_layout().apply_subscripts(subscripts)
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::view_type
strided_array_base<T>::transpose()
{
    return self_type(
        view_storage(), 
        get_layout().transpose()
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::const_view_type
strided_array_base<T>::transpose() const
{
    return self_type(
        view_storage(), 
        get_layout().transpose()
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::view_type
strided_array_base<T>::permute(span<std::size_t> order)
{
    return self_type(
        view_storage(), 
        get_layout().permute(order)
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::const_view_type
strided_array_base<T>::permute(span<std::size_t> order) const
{
    return self_type(
        view_storage(), 
        get_layout().permute(order)
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::view_type
strided_array_base<T>::swap_axes(std::size_t axis1, std::size_t axis2)
{
    return self_type(
        view_storage(), 
        get_layout().swap_axes(axis1, axis2)
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::const_view_type
strided_array_base<T>::swap_axes(std::size_t axis1, std::size_t axis2) const
{
    return self_type(
        view_storage(), 
        get_layout().swap_axes(axis1, axis2)
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::view_type
strided_array_base<T>::squeeze()
{
    return self_type(
        view_storage(), 
        get_layout().squeeze()
    );
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::const_view_type
strided_array_base<T>::squeeze() const
{
    return self_type(
        view_storage(), 
        get_layout().squeeze()
    );
}

template <typename T>
inline
void strided_array_base<T>::broadcast_extents_to_layout(std::vector<std::size_t> &extents,
                                                        std::size_t trailing_dimensions ) const
{
    get_layout().broadcast_extents_to_layout(extents, trailing_dimensions);
}

template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::view_type
strided_array_base<T>::broadcast_layout_to_extents(span<const std::size_t> extents,
                                                   std::size_t trailing_dimensions )
{
    return self_type(
        view_storage(), 
        get_layout().broadcast_layout_to_extents(extents, trailing_dimensions)
    );
}


template <typename T>
XMIPP4_NODISCARD inline
typename strided_array_base<T>::const_view_type
strided_array_base<T>::broadcast_layout_to_extents(span<const std::size_t> extents,
                                                    std::size_t trailing_dimensions ) const
{
    return self_type(
        view_storage(), 
        get_layout().broadcast_layout_to_extents(extents, trailing_dimensions)
    );
}



template <typename T>
inline
typename strided_array_base<T>::self_type* 
strided_array_base<T>::self() noexcept
{
    return static_cast<self_type*>(this);
}

template <typename T>
inline
const typename strided_array_base<T>::self_type* 
strided_array_base<T>::self() const noexcept
{
    return static_cast<const self_type*>(this);
}

template <typename T>
inline
const typename strided_array_base<T>::layout_type& 
strided_array_base<T>::get_layout() const noexcept
{
    return self()->get_layout();
}

template <typename T>
inline
typename strided_array_base<T>::storage_view_type
strided_array_base<T>::view_storage()
{
    return self()->view_storage();
}

template <typename T>
inline
typename strided_array_base<T>::storage_const_view_type
strided_array_base<T>::view_storage() const
{
    return self()->view_storage();
}

} // namespace multidimensional
} // namespace xmipp4
