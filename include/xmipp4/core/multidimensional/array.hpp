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
 * @date 2025-02-03
 * 
 */

#include <vector>
#include <memory>
#include <cstddef>

#include "dynamic_subscript.hpp"
#include "storage_traits.hpp"
#include "../span.hpp"
#include "../platform/attributes.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename Storage, typename Layout>
class array
    : public strided_array_base<array>
{
public:
    using storage_type = Storage;
    using layout_type = Layout;
    using storage_view_type = 
        typename storage_traits<storage_type>::view_type;
    using storage_const_view_type = 
        typename storage_traits<storage_type>::const_view_type;
    using view_type = array<storage_view_type, layout_type>;
    using const_view_type = array<storage_const_view_type, layout_type>;

    array() = default;

    /**
     * @brief Construct a new array object from components.
     * 
     * @param storage The storage used with the new array. 
     * @param layout The layout of the new array.
     */
    array(storage_type storage, layout_type layout);

    array(const array &other) = default;
    array(array&& other) = default;
    ~array() = default;

    array& operator=(const array &other) = default;
    array& operator=(array&& other) = default;



    /**
     * @brief Get the storage for this array.
     * 
     * @return const storage_type& Storage of this array.
     */
    XMIPP4_NODISCARD
    const storage_type& get_storage() const noexcept;

    /**
     * @brief Get the layout for this array.
     * 
     * @return const layout_type& Layout of this array.
     */
    XMIPP4_NODISCARD
    const layout_type& get_layout() const noexcept;

    /**
     * @brief Obtain a writable alias of the underlying storage.
     * 
     * @return storage_type Alias of the storage.
     */
    XMIPP4_NODISCARD
    storage_view_type view_storage();

    /**
     * @brief Obtain a read-only alias of the underlying storage.
     * 
     * @return storage_type Alias of the storage.
     */
    XMIPP4_NODISCARD
    storage_const_view_type view_storage() const;

    /**
     * @brief Get the rank of the array.
     * 
     * The rank of the array is the number of dimensions. Shorthand for
     * get_layout().get_rank()
     * 
     * @return std::size_t The rank.
     * 
     */
    XMIPP4_NODISCARD
    std::size_t get_rank() const noexcept;

private:
    storage_type m_storage;
    layout_type m_layout;

};

} // namespace multidimensional
} // namespace xmipp4

#include "array.inl"
