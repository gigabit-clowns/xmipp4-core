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
 * @file array.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines array class
 * @date 2024-03-19
 * 
 */

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
struct layout_traits;

template <typename T>
struct storage_traits;

template <typename L, typename S>
class array
{
public:
    using layout_type = L;
    using storage_type = S;
    using layout_traits = multidimensional::layout_traits<layout_type>;
    using storage_traits = multidimensional::storage_traits<storage_type>;
    using alias_type = array<layout_type, typename storage_traits::alias_type>;
    using const_alias_type = array<layout_type, typename storage_traits::const_alias_type>;

    const layout_type& get_layout() const noexcept;

private:
    layout_type m_layout;
    storage_type m_storage;

};

class dynamic_strided_layout;

template <typename T>
class shared_storage;

template <typename T>
using dynamic_array = array<dynamic_strided_layout, shared_storage<T>>;

} // namespace multidimensional
} // namespace xmipp4

#include "array.inl"
