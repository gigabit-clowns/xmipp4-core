// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "dynamic_subscript.hpp"
#include "strided_layout.hpp"
#include "../numerical_type.hpp"
#include "../span.hpp"
#include "../platform/attributes.hpp"

#include <array>
#include <memory>

namespace xmipp4 
{
namespace hardware
{

class buffer;

} // namespace hardware

namespace multidimensional
{

class strided_layout;

class array
{
public:
    using storage_type = hardware::buffer;

    array();
    array(strided_layout layout, 
          std::shared_ptr<storage_type> storage, 
          numerical_type data_type );
    array(const array& other) = delete;
    array(array&& other) noexcept;
    ~array();

    array& operator=(const array& other) = delete;
    array& operator=(array&& other) noexcept;

    /**
     * @brief Get the data type
     * 
     * @return numerical_type 
     */
    numerical_type get_data_type() const noexcept;

    /**
     * @brief Get the layout of this array.
     * 
     * @return strided_layout The layout.
     */
    strided_layout get_layout() const noexcept;

    /**
     * @brief Get the storage of this array.
     * 
     * @return storage_type* The storage.
     */
    storage_type* get_storage() noexcept;

    /**
     * @brief Get the storage of this array.
     * 
     * @return const storage_type* The storage.
     */
    const storage_type* get_storage() const noexcept;

    /**
     * @brief Get a ref-counted pointer to the storage of this array.
     * 
     * @return std::shared_ptr<storage_type> The storage.
     */
    XMIPP4_NODISCARD
    std::shared_ptr<storage_type> share_storage() noexcept;

    /**
     * @brief Get a ref-counted pointer to read only storage of this array.
     * 
     * @return std::shared_ptr<const storage_type> The storage.
     */
    XMIPP4_NODISCARD
    std::shared_ptr<const storage_type> share_storage() const noexcept;

    /**
     * @brief Apply a set of subscripts to this layout.
     * 
     * @tparam Args Types of the arguments. Must be std::ptrdiff_t, slice,
     * new_axis_tag or ellipsis_tag.
     * @param args Subscripts to be applied to this array.
     * @see apply_subscripts
     * @return array The resulting array.
     * 
     */
    template <typename... Args>
    XMIPP4_NODISCARD 
    array operator()(Args&& ...args)
    {
        const std::array<dynamic_subscript, sizeof...(Args)> subscripts = {
            dynamic_subscript(std::forward<Args>(args))...
        };
        return apply_subscripts(make_span(subscripts));
    }
    
    /**
     * @brief Get the rank of the array, i.e. the number of axes.
     * 
     * @return std::size_t The number of axes in the array.
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
     * @brief Returns a view to this array. 
     * 
     * @return array The view to this array.
     */
    XMIPP4_NODISCARD
    array view() noexcept;

    /**
     * @brief Apply a set of dynamic subscripts to this layout.
     * 
     * @param subscripts The subscripts.
     * @return array The resulting layout.
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    array apply_subscripts(span<const dynamic_subscript> subscripts);

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return array The resulting array.
     */
    XMIPP4_NODISCARD
    array transpose();

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return array Permuted array.
     * @throws std::invalid_argument If the permutation order is invalid.
     */
    XMIPP4_NODISCARD
    array permute(span<const std::size_t> order);
    
    /**
     * @brief Swap the position of two axes of the array.
     * 
     * @param axis1 Index of the first axis. Must be in [-rank, rank).
     * Negative values indicate axes begining from the end.
     * @param axis2 Index of the second axis. Must be in [-rank, rank).
     * Negative values indicate axes begining from the end.
     * @return array Permuted array.
     * @throws std::out_of_bounds If either axis1 or axis2 exceeds bounds.
     */
    XMIPP4_NODISCARD
    array matrix_transpose(
        std::ptrdiff_t axis1 = -2, 
        std::ptrdiff_t axis2 = -1
    );
    
    /**
     * @brief Obtain a view of the matrix diagonal defined by the provided
     * two axes.
     * 
     * @param axis1 Index of the first axis. Must be in [-rank, rank).
     * Negative values indicate axes begining from the end.
     * @param axis2 Index of the second axis. Must be in [-rank, rank).
     * Negative values indicate axes begining from the end. Can not semantically
     * equal axis1.
     * @return array A view to the diagonal.
     * @throws std::out_of_bounds If either axis1 or axis2 exceeds bounds.
     * @throws std::invalid_argument if axis1 equals axis2
     */
    XMIPP4_NODISCARD
    array matrix_diagonal(
        std::ptrdiff_t axis1 = -2, 
        std::ptrdiff_t axis2 = -1
    );

    /**
     * @brief Remove insignificant axes from the layout.
     * 
     * @return array The resulting array.
     */
    XMIPP4_NODISCARD
    array squeeze();

    /**
     * @brief Perform a broadcast of the array to match the provided extents.
     * 
     * This function modifies the layout to match the provided extents by 
     * adding phantom axes or adjusting existing axes as needed upto the last 
     * trailing_dimensions. 
     * 
     * @param extents Extents to broadcast to.
     * untouched. Must be greater or equal to rank. Defaults to zero.
     * @return array The resulting broadcasted array.
     * @throws std::invalid_argument If the layout has more axes than extents.
     * @throws std::invalid_argument If the axes cannot be broadcasted to the 
     * provided extents.
     */
    XMIPP4_NODISCARD 
    array broadcast_to(span<const std::size_t> extents);

private:
    class implementation;
    std::shared_ptr<implementation> m_implementation;

    explicit array(std::shared_ptr<implementation> impl) noexcept;
    explicit array(implementation &&impl);

};

} // namespace multidimensional
} // namespace xmipp4
