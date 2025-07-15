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
 * @file strided_array_base.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines strided_array_base class
 * @date 2025-07-15
 * 
 */

#include <vector>
#include <cstddef>

#include "dynamic_subscript.hpp"
#include "../span.hpp"
#include "../platform/attributes.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class strided_array_base
{
public:
    using self_type = T;
    using storage_type = typename self_type::storage_type;
    using layout_type = typename self_type::layout_type;
    using view_type = typename self_type::view_type;
    using const_view_type = typename self_type::const_view_type;
    using storage_view_type = typename self_type::storage_view_type;
    using storage_const_view_type = typename self_type::storage_const_view_type;

    strided_array_base() = default;
    strided_array_base(const strided_array_base &other) = default;
    strided_array_base(strided_array_base&& other) = default;
    ~strided_array_base() = default;

    strided_array_base& operator=(const strided_array_base &other) = default;
    strided_array_base& operator=(strided_array_base&& other) = default;

    /**
     * @brief Apply a set of dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return view_type Selected subset of the strided_array_base
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
     * @return const_view_type Selected subset of the strided_array_base
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    const_view_type
    apply_subscripts(span<const dynamic_subscript> subscripts) const;

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return view_type Transposed strided_array_base.
     */
    XMIPP4_NODISCARD
    view_type transpose();

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return const_view_type Transposed strided_array_base.
     */
    XMIPP4_NODISCARD
    const_view_type transpose() const;

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return view_type The permuted strided_array_base.
     */
    XMIPP4_NODISCARD
    view_type permute(span<std::size_t> order);

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return const_view_type The permuted strided_array_base.
     */
    XMIPP4_NODISCARD
    const_view_type permute(span<std::size_t> order) const;

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return view_type strided_array_base with the axes swapped.
     */
    XMIPP4_NODISCARD
    view_type swap_axes(std::size_t axis1, std::size_t axis2);

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return const_view_type strided_array_base with the axes swapped.
     */
    XMIPP4_NODISCARD
    const_view_type swap_axes(std::size_t axis1, std::size_t axis2) const;

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return const_view_type Squeezed strided_array_base.
     */
    XMIPP4_NODISCARD
    view_type squeeze();

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return const_view_type Squeezed strided_array_base.
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
     * @return view_type The resulting broadcasted strided_array_base.
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
     * @return const_view_type The resulting broadcasted strided_array_base.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    const_view_type
    broadcast_layout_to_extents(span<const std::size_t> extents,
                                std::size_t trailing_dimensions = 0 ) const;

private:
    self_type* self() noexcept;
    const self_type* self() const noexcept;
    const layout_type& get_layout() const noexcept;
    storage_view_type view_storage();
    storage_const_view_type view_storage() const;

};

} // namespace multidimensional
} // namespace xmipp4

#include "strided_array_base.inl"
