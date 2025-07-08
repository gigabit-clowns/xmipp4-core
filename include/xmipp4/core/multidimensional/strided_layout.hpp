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
 * @file strided_layout.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines strided_layout class
 * @date 2025-02-03
 * 
 */

#include <vector>
#include <cstddef>

#include "strided_axis.hpp"
#include "dynamic_subscript.hpp"
#include "../span.hpp"
#include "../platform/attributes.hpp"

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Class describing element layout on a multidimensional array
 * 
 */
class strided_layout
{
public:
    strided_layout() = default;

    /**
     * @brief Construct a dynamic layout from its components.
     * 
     * @param axes Axes used in the dynamic layout
     * @param offset Offset used in the dynamic layout
     */
    strided_layout(std::vector<strided_axis> &&axes,
                   std::ptrdiff_t offset ) noexcept;

    /**
     * @brief Construct a new column-major dynamic layout.
     * 
     * @param extents Array with the extents of the layout. The first rank
     * elements must be dereferenceable.
     * @param rank Number of elements in the arrays.
     * @param offset Offset of the layout. Defaults to zero.
     * 
     */
    strided_layout(const std::size_t *extents, 
                   std::size_t rank,
                   std::ptrdiff_t offset = 0 );
    /**
     * @brief Construct a new dynamic layout from its components.
     * 
     * @param extents Array with the extents of the layout. The first rank
     * elements must be dereferenceable.
     * @param strides Array with the strides of the layout. The first rank
     * elements must be dereferenceable.
     * @param rank Number of elements in the arrays.
     * @param offset Offset of the layout. Defaults to zero.
     * 
     */
    strided_layout(const std::size_t *extents, 
                   const std::ptrdiff_t *strides, 
                   std::size_t rank,
                   std::ptrdiff_t offset = 0 );

    /**
     * @brief Construct a new dynamic layout from a list of axes.
     * 
     * @param axes Array with axis descriptors.
     * @param rank Number of elements in the array.
     * @param offset Offset of the layout. Defaults to zero.
     * 
     */
    strided_layout(const strided_axis *axes, 
                   std::size_t rank,
                   std::ptrdiff_t offset = 0 );

    strided_layout(const strided_layout &other) = default;
    strided_layout(strided_layout &&other) = default;
    ~strided_layout() = default;

    strided_layout& operator=(const strided_layout &other) = default;
    strided_layout& operator=(strided_layout &&other) = default;

    /**
     * @brief Get the rank of the layout.
     * 
     * The rank of a layout is the number of dimensions.
     * 
     * @return std::size_t The rank.
     * 
     */
    XMIPP4_NODISCARD
    std::size_t get_rank() const noexcept;

    /**
     * @brief Get the axis parameters component wise.
     * 
     * @param extents Array of std::size_t-s. May be null. In case it is not, it
     * must be dereferenceable in [extents, extents+count).
     * @param strides Array of std::ptrdiff_t-s. May be null. In case it is not, 
     * it must be dereferenceable in [strides, strides+count).
     * @param count Element counts of the arrays.
     * @return std::size_t Number of elements written; min(rank, count).
     * 
     */
    std::size_t get_axes(std::size_t *extents, 
                         std::ptrdiff_t *strides,
                         std::size_t count ) const noexcept;

    /**
     * @brief Get the axis parameters for one of the axes.
     * 
     * @param index Index of the axis.
     * @param out Axis descriptor where the requested axis is written.
     * @return true Index is valid and axis descriptor was written.
     * @return false Index is out of bounds and the output was not written.
     * 
     */
    bool get_axis(std::size_t index, strided_axis &out) const noexcept;

    /**
     * @brief Get the offset of the layout.
     * 
     * @return std::size_t The offset.
     */
    XMIPP4_NODISCARD
    std::ptrdiff_t get_offset() const noexcept;

    /**
     * @brief Computes the minimum storage size (in elements) for this layout.
     * 
     * @return std::size_t Minimum storage size in elements.
     */
    XMIPP4_NODISCARD
    std::size_t compute_storage_requirement() const noexcept;


    /**
     * @brief Apply a set of dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return strided_layout The resulting layout.
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    strided_layout 
    apply_subscripts(span<const dynamic_subscript> subscripts) const;

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return strided_layout The resulting layout.
     */
    XMIPP4_NODISCARD
    strided_layout transpose() const;

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return strided_layout Permuted layout.
     * @throws std::invalid_argument If the permutation order is invalid.
     */
    XMIPP4_NODISCARD
    strided_layout permute(span<const std::size_t> order) const;

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return strided_layout Permuted layout.
     * @throws std::invalid_argument If either axis1 or axis2 exceeds bounds.
     */
    XMIPP4_NODISCARD
    strided_layout swap_axes(std::size_t axis1, std::size_t axis2) const;

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return strided_layout The resulting layout.
     */
    XMIPP4_NODISCARD
    strided_layout squeeze() const;

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
    void broadcast_dry(std::vector<std::size_t> &extents,
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
     * @return strided_layout The resulting broadcasted layout.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    strided_layout broadcast_to(span<const std::size_t> extents,
                                std::size_t trailing_dimensions = 0 ) const;

private:
    std::vector<strided_axis> m_axes;
    std::ptrdiff_t m_offset;

    class apply_subscripts_helper;

};



template <typename T>
struct layout_traits;

template <>
struct layout_traits<strided_layout>
{

};

} // namespace multidimensional
} // namespace xmipp4

#include "strided_layout.inl"
