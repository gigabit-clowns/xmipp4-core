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

#include "../span.hpp"
#include "../platform/attributes.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
struct storage_traits;

template <typename T>
struct layout_traits;



template <typename Storage, typename Layout>
class array
{
public:
    using storage_type = Storage;
    using layout_type = Layout;

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
     * @brief Get the rank of the array.
     * 
     * The rank of the array is the number of dimensions.
     * 
     * @return std::size_t The rank.
     * 
     */
    XMIPP4_NODISCARD
    std::size_t get_rank() const noexcept;

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return array The resulting layout.
     */
    XMIPP4_NODISCARD
    array transpose() const;

    /**
     * @brief Reverse the order of the axes in-place.
     * 
     * @return array& *this
     */
    array& transpose_inplace() noexcept;

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return array Permuted layout.
     */
    XMIPP4_NODISCARD
    array permute(span<std::size_t> order) const;

    /**
     * @brief Permute the order of the axes in-place.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return array& *this
     */
    array& permute_inplace(span<std::size_t> order);

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return array Permuted layout.
     */
    XMIPP4_NODISCARD
    array swap_axes(std::size_t axis1, std::size_t axis2) const;

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return array& *this
     */
    array& swap_axes_inplace(std::size_t axis1, std::size_t axis2);

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return array The resulting layout.
     */
    XMIPP4_NODISCARD
    array squeeze() const;

    /**
     * @brief Remove insignificant axes of the layout in-place.
     * 
     * @return array& *this
     */
    array& squeeze_inplace() noexcept;

    /**
     * @brief Merge contiguous axes.
     * 
     * @return array Raveled layout.
     */
    XMIPP4_NODISCARD
    array ravel() const;

    /**
     * @brief Merge contiguous axes in-place.
     * 
     * @return array& Raveled layout.
     */
    array& ravel_inplace();

private:
    storage_type m_storage;
    layout_type m_layout;

};

} // namespace multidimensional
} // namespace xmipp4

#include "array.inl"
