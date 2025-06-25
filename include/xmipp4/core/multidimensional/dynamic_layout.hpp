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
 * @file dynamic_layout.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines dynamic_layout class
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
class dynamic_layout
{
public:
    dynamic_layout() = default;

    /**
     * @brief Construct a new column-major dynamic layout.
     * 
     * @param extents Array with the extents of the layout. The first rank
     * elements must be dereferenceable.
     * @param rank Number of elements in the arrays.
     * @param offset Offset of the layout. Defaults to zero.
     * 
     */
    dynamic_layout(const std::size_t *extents, 
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
    dynamic_layout(const std::size_t *extents, 
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
    dynamic_layout(const strided_axis *axes, 
                   std::size_t rank,
                   std::ptrdiff_t offset = 0 );

    dynamic_layout(const dynamic_layout &other) = default;
    dynamic_layout(dynamic_layout &&other) = default;
    ~dynamic_layout() = default;

    dynamic_layout& operator=(const dynamic_layout &other) = default;
    dynamic_layout& operator=(dynamic_layout &&other) = default;

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
     * @brief Apply a set of dynamic dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return dynamic_layout The resulting layout.
     * @throws std::runtime_error If an ellipsis subscript is encountered (not implemented).
     * @throws std::invalid_argument If not all subscripts are processed.
     */
    XMIPP4_NODISCARD
    dynamic_layout apply_subscripts(span<dynamic_subscript> subscripts) const;

    /**
     * @brief Apply a set of dynamic dynamic subscripts to this layout in-place.
     * 
     * @param subscripts The subscripts.
     * @return dynamic_layout& The resulting layout.
     * @throws std::runtime_error If an ellipsis subscript is encountered (not implemented).
     * @throws std::invalid_argument If not all subscripts are processed.
     */
    dynamic_layout& apply_subscripts_inplace(span<dynamic_subscript> subscripts);

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return dynamic_layout The resulting layout.
     */
    XMIPP4_NODISCARD
    dynamic_layout transpose() const;

    /**
     * @brief Reverse the order of the axes in-place.
     * 
     * @return dynamic_layout& *this
     */
    dynamic_layout& transpose_inplace() noexcept;

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return dynamic_layout Permuted layout.
     * @throws std::invalid_argument If the permutation order is invalid.
     */
    XMIPP4_NODISCARD
    dynamic_layout permute(span<const std::size_t> order) const;

    /**
     * @brief Permute the order of the axes in-place.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return dynamic_layout& *this
     * @throws std::invalid_argument If the permutation order is invalid.
     */
    dynamic_layout& permute_inplace(span<const std::size_t> order);

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return dynamic_layout Permuted layout.
     * @throws std::invalid_argument If either axis1 or axis2 exceeds bounds.
     */
    XMIPP4_NODISCARD
    dynamic_layout swap_axes(std::size_t axis1, std::size_t axis2) const;

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return dynamic_layout& *this
     * @throws std::invalid_argument If either axis1 or axis2 exceeds bounds.
     */
    dynamic_layout& swap_axes_inplace(std::size_t axis1, std::size_t axis2);

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return dynamic_layout The resulting layout.
     */
    XMIPP4_NODISCARD
    dynamic_layout squeeze() const;

    /**
     * @brief Remove insignificant axes of the layout in-place.
     * 
     * @return dynamic_layout& *this
     */
    dynamic_layout& squeeze_inplace() noexcept;

    /**
     * @brief Reduce the rank of the layout as much as possible by combining
     * contiguous axes.
     * 
     * Unlike numpy's ravel, this will only combine axes when possible.
     * 
     * @return dynamic_layout The resulting layout.
     */
    XMIPP4_NODISCARD
    dynamic_layout coalesce_axes() const;

    /**
     * @brief Reduce the rank of the layout as much as possible by combining
     * contiguous axes.
     * 
     * Unlike numpy's ravel, this will only combine axes when possible.
     * 
     * @return dynamic_layout& *this
     */
    dynamic_layout& coalesce_axes_inplace() noexcept;

    /**
     * @brief Perform a broadcast between the layout and the provided extents.
     * 
     * This function modifies the layout to match the provided extents by 
     * adding phantom axes or adjusting existing axes as needed. Similarly,
     * it will add or modify extents of size 1 to match the layout.
     * 
     * @param extents Extents to broadcast to.
     * @return dynamic_layout The resulting broadcasted layout.
     * @throws std::invalid_argument If the axes cannot be broadcasted to 
     * the provided extents.
     */
    dynamic_layout broadcast(std::vector<std::size_t> &extents) const;

    /**
     * @brief Perform a broadcast between the layout and the provided extents
     * in-place.
     * 
     * This function modifies the layout to match the provided extents by 
     * adding phantom axes or adjusting existing axes as needed. Similarly,
     * it will add or modify extents of size 1 to match the layout.
     * 
     * @param extents Extents to broadcast to.
     * @return dynamic_layout& The modified layout.
     * @throws std::invalid_argument If the axes cannot be broadcasted to 
     * the provided extents.
     */
    dynamic_layout& broadcast_inplace(std::vector<std::size_t> &extents);

    /**
     * @brief Perform a broadcast of the layout to match the provided extents.
     * 
     * This function modifies the layout to match the provided extents by 
     * adding phantom axes or adjusting existing axes as needed.
     * 
     * @param extents Extents to broadcast to.
     * @return dynamic_layout The resulting broadcasted layout.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    dynamic_layout broadcast_to(span<const std::size_t> extents) const;

    /**
     * @brief Perform a broadcast of the layout to match the provided extents in-place.
     * 
     * This function modifies the current layout to match the provided extents
     * by adding phantom axes or adjusting existing axes as needed.
     * 
     * @param extents Extents to broadcast to.
     * @return dynamic_layout& The modified layout.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to 
     * the provided extents.
     */
    dynamic_layout& broadcast_to_inplace(span<const std::size_t> extents);

private:
    std::vector<strided_axis> m_axes;
    std::ptrdiff_t m_offset;

};



template <typename T>
struct layout_traits;

template <>
struct layout_traits<dynamic_layout>
{

};

} // namespace multidimensional
} // namespace xmipp4

#include "dynamic_layout.inl"
