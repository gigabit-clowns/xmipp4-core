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
#include "dynamic_subscript.hpp"
#include "layout_flags.hpp"
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
    XMIPP4_NODISCARD
    std::ptrdiff_t get_offset() const noexcept;

    /**
     * @brief Get the flags for this layout.
     * 
     * @return layout_flags The properties of this flag.
     */
    XMIPP4_NODISCARD
    layout_flags get_flags() const noexcept;

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
     */
    XMIPP4_NODISCARD
    layout_reference apply_subscripts(span<dynamic_subscript> subscripts) const;

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
     */
    XMIPP4_NODISCARD
    layout_reference permute(span<std::size_t> order) const;

    /**
     * @brief Swap two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return layout_reference Permuted layout.
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
     * @brief Reduce the rank of the layout as much as possible by combining
     * contiguous axes.
     * 
     * Unlike numpy's coalesce_axes, this will only combine axes when possible.
     * 
     * @return layout_reference The resulting layout.
     */
    XMIPP4_NODISCARD
    layout_reference coalesce_axes() const;

    /**
     * @brief Perform a broadcast between the layout and the provided extents.
     * 
     * @param extents Extents to broadcast to.
     * @return layout_reference The resulting broadcasted layout.
     * @throws std::invalid_argument If the axes cannot be broadcasted to 
     * the provided extents.
     */
    XMIPP4_NODISCARD
    layout_reference broadcast(std::vector<std::size_t> &extents) const;

    /**
     * @brief Perform a broadcast of the layout to match the provided extents.
     * 
     * @param extents Extents to broadcast to.
     * @return layout_reference The resulting broadcasted layout.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    XMIPP4_NODISCARD
    layout_reference broadcast_to(span<const std::size_t> extents) const;

private:
    std::shared_ptr<layout_type> m_layout;

    template <typename Func, typename... Args>
    layout_reference apply(Func &&func, Args&& ...args);

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
