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

#include "axis_descriptor.hpp"
#include "../slice.hpp"
#include "../span.hpp"

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
    dynamic_layout(const axis_descriptor *axes, 
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
    std::size_t get_rank() const noexcept;

    /**
     * @brief Get the axis parameters for one of the axes.
     * 
     * @param index Index of the axis.
     * @param out Axis descriptor where the requested axis is written.
     * @return true Index is valid and axis descriptor was written.
     * @return false Index is out of bounds and the output was not written.
     * 
     */
    bool get_axis(std::size_t index, axis_descriptor &out) const noexcept;

    /**
     * @brief Get the offset of the layout.
     * 
     * @return std::size_t The offset.
     */
    std::ptrdiff_t get_offset() const noexcept;



    /**
     * @brief Apply slices to the first dimensions.
     * 
     * @param slices Sequence of slice objects. Must be less or equal
     * than the rank of this layout.
     * 
     * @return dynamic_layout Sliced layout
     * 
     */
    dynamic_layout slice(span<const dynamic_slice> slices);

    /**
     * @brief Apply slices to the first dimensions in-place.
     * 
     * @param slices Sequence of slice objects. Must be less or equal
     * than the rank of this layout.
     * 
     * @return dynamic_layout& *this
     * 
     */
    dynamic_layout& slice_inplace(span<const dynamic_slice> slices);

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return dynamic_layout The resulting layout.
     */
    dynamic_layout transpose() const;

    /**
     * @brief Reverse the order of the axes in-place.
     * 
     * @return dynamic_layout& *this
     */
    dynamic_layout& transpose_inplace() noexcept;

    /**
     * @brief Reverse the order of the last two axes.
     * 
     * @return dynamic_layout The resulting layout.
     */
    dynamic_layout matrix_transpose() const;

    /**
     * @brief Reverse the order of the last two axes in-place.
     * 
     * @return dynamic_layout& *this
     */
    dynamic_layout& matrix_transpose_inplace() noexcept;

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return dynamic_layout The resulting layout.
     */
    dynamic_layout squeeze() const;

    /**
     * @brief Remove insignificant axes of the layout in-place.
     * 
     * @return dynamic_layout& *this
     */
    dynamic_layout& squeeze_inplace() noexcept;

private:
    std::vector<axis_descriptor> m_axes;
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
