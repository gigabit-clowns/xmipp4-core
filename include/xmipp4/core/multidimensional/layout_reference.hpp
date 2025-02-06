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
 * @file layout_reference.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines layout_reference class
 * @date 2025-02-03
 * 
 */

#include <memory>
#include <cstddef>

#include "axis_descriptor.hpp"
#include "../slice.hpp"
#include "../span.hpp"

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Class describing a reference to a layout.adjacent
 * 
 * This reference uses Copy on Write (CoW) semantics, so that the underlying
 * layout is copied on write operations.
 * 
 */
template <typename T>
class layout_reference
{
public:
    using layout_type = T;

    layout_reference() = default;

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
    layout_reference(const std::size_t *extents, 
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
    layout_reference(const axis_descriptor *axes, 
                     std::size_t rank,
                     std::ptrdiff_t offset = 0 );

    /**
     * @brief Construct a new layout reference as a copy of an undelying layout.
     * 
     * @param other The layout to be copied.
     */
    explicit layout_reference(const layout_type &other);

    /**
     * @brief Construct a new layout reference moving the undelying layout.
     * 
     * @param other The layout to be moved.
     */
    explicit layout_reference(layout_type &&other);

    layout_reference(const layout_reference &other) = default;
    layout_reference(layout_reference &&other) = default;
    ~layout_reference() = default;

    layout_reference& operator=(const layout_reference &other) = default;
    layout_reference& operator=(layout_reference &&other) = default;

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
     * @brief Reverse the order of the axes.
     * 
     * @return layout_reference The resulting layout.
     */
    layout_reference transpose() const;

    /**
     * @brief Reverse the order of the axes in-place.
     * 
     * @return layout_reference& *this
     */
    layout_reference& transpose_inplace() noexcept;

    /**
     * @brief Reverse the order of the last two axes.
     * 
     * @return layout_reference The resulting layout.
     */
    layout_reference matrix_transpose() const;

    /**
     * @brief Reverse the order of the last two axes in-place.
     * 
     * @return layout_reference& *this
     */
    layout_reference& matrix_transpose_inplace() noexcept;

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return layout_reference The resulting layout.
     */
    layout_reference squeeze() const;

    /**
     * @brief Remove insignificant axes of the layout in-place.
     * 
     * @return layout_reference& *this
     */
    layout_reference& squeeze_inplace() noexcept;

private:
    std::shared_ptr<layout_type> m_layout;

    void copy_on_write();

};



template <typename T>
struct layout_traits;

template <typename T>
struct layout_traits<layout_reference<T>>
{

};

} // namespace multidimensional
} // namespace xmipp4

#include "layout_reference.inl"
