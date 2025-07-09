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
{
public:
    using storage_type = Storage;
    using layout_type = Layout;
    using view_type = 
        array<typename storage_traits<storage_type>::view_type, layout_type>;
    using const_view_type = 
        array<typename storage_traits<storage_type>::const_view_type, layout_type>;

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
     * The rank of the array is the number of dimensions. Shorthand for
     * get_layout().get_rank()
     * 
     * @return std::size_t The rank.
     * 
     */
    XMIPP4_NODISCARD
    std::size_t get_rank() const noexcept;



    /**
     * @brief Apply a set of dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return view_type Selected subset of the array
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    view_type
    apply_subscripts(span<const dynamic_subscript> subscripts);

    /**
     * @brief Apply a set of dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return const_view_type Selected subset of the array
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    const_view_type
    apply_subscripts(span<const dynamic_subscript> subscripts) const;

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return view_type Transposed array.
     */
    XMIPP4_NODISCARD
    view_type transpose();

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return const_view_type Transposed array.
     */
    XMIPP4_NODISCARD
    const_view_type transpose() const;

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return view_type The permuted array.
     */
    XMIPP4_NODISCARD
    view_type permute(span<std::size_t> order);

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return const_view_type The permuted array.
     */
    XMIPP4_NODISCARD
    const_view_type permute(span<std::size_t> order) const;

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return view_type Array with the axes swapped.
     */
    XMIPP4_NODISCARD
    view_type swap_axes(std::size_t axis1, std::size_t axis2);

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return const_view_type Array with the axes swapped.
     */
    XMIPP4_NODISCARD
    const_view_type swap_axes(std::size_t axis1, std::size_t axis2) const;

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return const_view_type Squeezed array.
     */
    XMIPP4_NODISCARD
    view_type squeeze();

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return const_view_type Squeezed array.
     */
    XMIPP4_NODISCARD
    const_view_type squeeze() const;

    /**
     * @brief Broadcast a shape to match this layout.
     * 
     * This function modifies the provided extents to match this layout upto
     * the last trailing_dimensions. The last trailing dimensions are not 
     * considered in the algorithm. This is useful to broadcast batch dimensions
     * while keeping trailing dimensions.
     * 
     * @param extents Extents to be broadcasted. Input/output parameter.
     * @param trailing_dimensions Number of dimensions at the end left 
     * untouched. Must be greater or equal to rank. Defaults to zero.
     * @throws std::invalid_argument If the extents has more axes than this
     * layout.
     * @throws std::invalid_argument If the extent can not be broadcasted to 
     * this layout.
     */
    void broadcast_extents_to_layout(std::vector<std::size_t> &extents,
                                     std::size_t trailing_dimensions = 0) const;

    /**
     * @brief Perform a broadcast of the layout to match the provided extents.
     * 
     * This function modifies the layout to match the provided extents by 
     * adding phantom axes or adjusting existing axes as needed upto the last 
     * trailing_dimensions. The last trailing dimensions from this layout are 
     * not considered in the algorithm and they are passed-through to the 
     * result. This is useful to broadcast batch dimensions while keeping
     * trailing dimensions.
     * 
     * @param extents Extents to broadcast to.
     * @param trailing_dimensions Number of dimensions at the end left 
     * untouched. Must be greater or equal to rank. Defaults to zero.
     * @return view_type The resulting broadcasted array.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    view_type
    broadcast_layout_to_extents(span<const std::size_t> extents,
                                std::size_t trailing_dimensions = 0 );

    /**
     * @brief Perform a broadcast of the layout to match the provided extents.
     * 
     * This function modifies the layout to match the provided extents by 
     * adding phantom axes or adjusting existing axes as needed upto the last 
     * trailing_dimensions. The last trailing dimensions from this layout are 
     * not considered in the algorithm and they are passed-through to the 
     * result. This is useful to broadcast batch dimensions while keeping
     * trailing dimensions.
     * 
     * @param extents Extents to broadcast to.
     * @param trailing_dimensions Number of dimensions at the end left 
     * untouched. Must be greater or equal to rank. Defaults to zero.
     * @return const_view_type The resulting broadcasted array.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    const_view_type
    broadcast_layout_to_extents(span<const std::size_t> extents,
                                std::size_t trailing_dimensions = 0 ) const;

private:
    storage_type m_storage;
    layout_type m_layout;

};

} // namespace multidimensional
} // namespace xmipp4

#include "array.inl"
