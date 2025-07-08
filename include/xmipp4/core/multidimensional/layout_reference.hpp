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

#include "strided_axis.hpp"
#include "dynamic_subscript.hpp"
#include "slice.hpp"
#include "../span.hpp"
#include "../platform/attributes.hpp"

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Class describing a reference to a layout.
 * 
 * This reference uses Copy on Write (CoW) semantics, so that the underlying
 * layout is copied on write operations. When empty, it represents a
 * default constructed layout, this is, no axes and zero offset.
 * 
 */
template <typename T>
class layout_reference
{
public:
    using layout_type = T;

    layout_reference() = default;

    /**
     * @brief Construct a new layout reference and the undelying layout.
     * 
     * @tparam Args Types of the arguments used with the constructor.
     * @param args Arguments forwarded to the underlying layout's constructor.
     */
    template <typename... Args>
    explicit layout_reference(Args&& ...args);

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
     * @param index Index o:f the axis.
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
     * @brief Get a read-only shared_ptr to the underlying layout.
     * 
     * @return std::shared_ptr<const layout_type> The underlying layout.
     */
    XMIPP4_NODISCARD
    std::shared_ptr<const layout_type> get_shared() const noexcept;

    /**
     * @brief Apply a set of dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return layout_reference The resulting layout.
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    layout_reference 
    apply_subscripts(span<const dynamic_subscript> subscripts) const;

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return layout_reference The resulting layout.
     */
    XMIPP4_NODISCARD
    layout_reference transpose() const;

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return layout_reference Permuted layout.
     * @throws std::invalid_argument If the permutation order is invalid.
     */
    XMIPP4_NODISCARD
    layout_reference permute(span<const std::size_t> order) const;

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return layout_reference Permuted layout.
     * @throws std::invalid_argument If either axis1 or axis2 exceeds bounds.
     */
    XMIPP4_NODISCARD
    layout_reference swap_axes(std::size_t axis1, std::size_t axis2) const;

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return layout_reference The resulting layout.
     */
    XMIPP4_NODISCARD
    layout_reference squeeze() const;

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
     * @return layout_reference The resulting broadcasted layout.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    layout_reference 
    broadcast_layout_to_extents(span<const std::size_t> extents,
                                std::size_t trailing_dimensions = 0 ) const;


private:
    std::shared_ptr<layout_type> m_layout;

    template <typename Func, typename... Args>
    layout_reference apply(Func &&func, Args&& ...args);

    template <typename Func, typename... Args>
    layout_reference apply_no_empty(Func &&func, Args&& ...args);

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
