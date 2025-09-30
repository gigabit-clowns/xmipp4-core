// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "dynamic_subscript.hpp"
#include "../numerical_type.hpp"
#include "../span.hpp"
#include "../platform/attributes.hpp"

#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout
{
public:
    strided_layout() noexcept;
    strided_layout(const strided_layout &other) noexcept;
    strided_layout(strided_layout &&other) noexcept;
    ~strided_layout();

    strided_layout& operator=(const strided_layout &other) noexcept;
    strided_layout& operator=(strided_layout &&other) noexcept;

    bool operator==(const strided_layout &other) const noexcept;
    bool operator!=(const strided_layout &other) const noexcept;

    /**
     * @brief Get the number of axis in the layout
     * 
     * @return std::size_t 
     */
    XMIPP4_NODISCARD
    std::size_t get_rank() const noexcept;

    /**
     * @brief Get the extents of the layout.
     * 
     * @param extents Output parameter where the extents are written.
     */
    void get_extents(std::vector<std::size_t> &extents) const;

    /**
     * @brief Get the strides of the layout.
     * 
     * @param extents Output parameter where the strides are written.
     */
    void get_strides(std::vector<std::ptrdiff_t> &strides) const;

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
     * @brief Computes the number of elements referenced by te layout.
     * 
     * @return std::size_t Number of elements.
     */
    XMIPP4_NODISCARD
    std::size_t compute_element_count() const noexcept;



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
    strided_layout swap_axes(std::ptrdiff_t axis1, std::ptrdiff_t axis2) const;

    /**
     * @brief Remove insignificant axes of the layout.
     * 
     * @return strided_layout The resulting layout.
     */
    XMIPP4_NODISCARD
    strided_layout squeeze() const;

    /**
     * @brief Perform a broadcast of the layout to match the provided extents.
     * 
     * This function modifies the layout to match the provided extents by 
     * adding phantom axes or adjusting existing axes as needed upto the last 
     * trailing_dimensions. 
     * 
     * @param extents Extents to broadcast to.
     * untouched. Must be greater or equal to rank. Defaults to zero.
     * @return strided_layout The resulting broadcasted layout.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    XMIPP4_NODISCARD
    strided_layout broadcast_to(span<const std::size_t> extents) const;

    /**
     * @brief Split the layout into an outer and inner layout.
     * 
     * Axes from [0, index) returned in the outer layout.
     * Axes from [index, end) are returned in the inner layout.
     * 
     * @param index Index at which the layout is split. Must be in 
     * [-extent, extent).
     * @return std::tuple<strided_layout, strided_layout> Tuple of 
     * (outer_layout, inner_layout)
     */
    XMIPP4_NODISCARD
    std::tuple<strided_layout, strided_layout> 
    split_at(std::ptrdiff_t index) const;

    /**
     * @brief Create a contiguous layout from the provided extents.
     * 
     * @param extents Extents of the layout.
     * @return strided_layout The resulting layout.
     */
    XMIPP4_NODISCARD
    static
    strided_layout make_contiguous_layout(span<const std::size_t> extents);

    XMIPP4_NODISCARD
    static
    strided_layout make_custom_layout(
        span<const std::size_t> extents, 
        span<const std::ptrdiff_t> strides, 
        std::ptrdiff_t offset = 0
    );

private:
    class implementation;
    std::shared_ptr<const implementation> m_implementation; // Copy-on-write

    explicit 
    strided_layout(std::shared_ptr<const implementation> impl) noexcept;
    explicit 
    strided_layout(implementation &&impl);

};

} // namespace multidimensional
} // namespace xmipp4
