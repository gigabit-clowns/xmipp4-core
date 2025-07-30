// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/attributes.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Derived>
class strided_array_interface
{
public:
    using self_type = Derived;
    using view_type = typename self_type::view_type;
    using const_view_type = typename self_type::const_view_type;

    strided_array_interface() = default;
    strided_array_interface(const strided_array_interface& other) = default;
    strided_array_interface(strided_array_interface&& other) = default;
    virtual ~strided_array_interface() = default;

    strided_array_interface& operator=(const strided_array_interface& other) = default;
    strided_array_interface& operator=(strided_array_interface&& other) = default;
    
    /**
     * @brief Get the number of dimensions (rank) of the array.
     * 
     * @return std::size_t The rank of the array.
     * 
     */
    virtual std::size_t get_rank() const noexcept = 0;

    /**
     * @brief Get the extents of the array.
     * 
     * If offset if greater or equal to the rank, the function will return
     * 0 a and extents nor strides will not be written.
     * Otherwise, it will write min(rank - offset, count) extents/strides.
     * 
     * @param extents Ouput array where the extents will be written. May be
     * null.
     * @param strides Ouput array where the strides will be written. May be 
     * null.
     * @param count Maximum number of extents/strides to retrieve.
     * @param offset First referenced axis.
     * @return std::size_t Number of extents retrieved. This will be
     * less than or equal to count, depending on the rank of the array.
     * 
     */
    virtual 
    std::size_t get_axis_parameters(std::size_t *extents,
                                    std::ptrdiff_t *strides,
                                    std::size_t count,
                                    std::size_t offset ) const noexcept = 0;

    /**
     * @brief Get the data type held by the array.
     * 
     * @return const std::type_info& typeinfo structure of the elements.
     * 
     */
    virtual const std::type_info& get_data_type() const noexcept = 0;

    /**
     * @brief Get the numeric type held by the array.
     * 
     * @return numerical_type The numeric type of the elements. 
     * numeric_type::unknown if the type is not one of the numeric types.
     * 
     */
    virtual numerical_type get_numeric_type() const noexcept = 0;

    /**
     * @brief Obtain a view to this array.
     * 
     * @return std::shared_ptr<view_type> The view to this array.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<view_type> view() = 0;
    
    /**
     * @brief Obtain a view to this array.
     * 
     * @return std::shared_ptr<const_view_type> The view to this array.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<const_view_type> view() const = 0;

    /**
     * @brief Apply a set of dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return std::shared_ptr<view_type> The resulting layout.
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<view_type> 
    apply_subscripts(span<const dynamic_subscript> subscripts) = 0;
    
    /**
     * @brief Apply a set of dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return std::shared_ptr<const_view_type> The resulting layout.
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<const_view_type> 
    apply_subscripts(span<const dynamic_subscript> subscripts) const = 0;

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return std::shared_ptr<view_type> The resulting layout.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<view_type> transpose() = 0;

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return std::shared_ptr<const_view_type> The resulting layout.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<const_view_type> transpose() const = 0;

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return std::shared_ptr<view_type> Permuted layout.
     * @throws std::invalid_argument If the permutation order is invalid.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<view_type> 
    permute(span<const std::size_t> order) = 0;

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return std::shared_ptr<view_type> Permuted layout.
     * @throws std::invalid_argument If the permutation order is invalid.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<const_view_type> 
    permute(span<const std::size_t> order) const = 0;

    /**
     * @brief Swap the position of two axes of the array.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return std::shared_ptr<view_type> Permuted layout.
     * @throws std::invalid_argument If either axis1 or axis2 exceeds bounds.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<view_type> 
    swap_axes(std::size_t axis1, std::size_t axis2) = 0;

    /**
     * @brief Swap the position of two axes of the array.
     * 
     * @param axis1 Index of the first axis. Must be in [0, rank).
     * @param axis2 Index of the second axis. Must be in [0, rank).
     * @return std::shared_ptr<const_view_type> Permuted layout.
     * @throws std::invalid_argument If either axis1 or axis2 exceeds bounds.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<const_view_type> 
    swap_axes(std::size_t axis1, std::size_t axis2) const = 0;

    /**
     * @brief Remove insignificant axes from the layout.
     * 
     * @return std::shared_ptr<view_type> The resulting layout.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<view_type> squeeze() = 0;

    /**
     * @brief Remove insignificant axes from the layout.
     * 
     * @return std::shared_ptr<const_view_type> The resulting layout.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<const_view_type> squeeze() const = 0;

    /**
     * @brief Broadcast a shape to match this array.
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
    virtual void 
    broadcast_extents_to_layout(std::vector<std::size_t> &extents,
                                std::size_t trailing_dimensions = 0) const = 0;

    /**
     * @brief Perform a broadcast of the array to match the provided extents.
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
     * @return std::shared_ptr<view_type> The resulting broadcasted layout.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<view_type> 
    broadcast_layout_to_extents(span<const std::size_t> extents,
                                std::size_t trailing_dimensions = 0 ) = 0;

    /**
     * @brief Perform a broadcast of the array to match the provided extents.
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
     * @return std::shared_ptr<const_view_type> The resulting broadcasted layout.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    XMIPP4_NODISCARD
    virtual std::shared_ptr<const_view_type> 
    broadcast_layout_to_extents(span<const std::size_t> extents,
                                std::size_t trailing_dimensions = 0 ) const = 0;

};

} // namespace multidimensional
} // namespace xmipp4
