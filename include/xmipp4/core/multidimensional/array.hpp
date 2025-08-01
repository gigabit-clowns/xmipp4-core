// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "dynamic_subscript.hpp"
#include "../numerical_type.hpp"
#include "../span.hpp"
#include "../platform/attributes.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class storage;
class strided_layout;

class array
{
public:
    array();
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
     * @return storage* The storage.
     */
    storage* get_storage() noexcept;

    /**
     * @brief Get a ref-counted pointer to the storage of this array.
     * 
     * @return std::shared_ptr<storage> The storage.
     */
    XMIPP4_NODISCARD
    std::shared_ptr<storage> share_storage() noexcept;

    /**
     * @brief Get a ref-counted pointer to read only storage of this array.
     * 
     * @return std::shared_ptr<const storage> The storage.
     */
    XMIPP4_NODISCARD
    std::shared_ptr<const storage> share_storage() const noexcept;



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
     * @return std::shared_ptr<array> The resulting layout.
     * @throws std::invalid_argument If not all subscripts are processed.
     * Or subscript is out of bounds
     */
    XMIPP4_NODISCARD
    array apply_subscripts(span<const dynamic_subscript> subscripts);

    /**
     * @brief Reverse the order of the axes.
     * 
     * @return std::shared_ptr<array> The resulting layout.
     */
    XMIPP4_NODISCARD
    array transpose();

    /**
     * @brief Permute the order of the axes.
     * 
     * @param order Order acquired by the new layout. Must have the same 
     * size as the amount of dimensions and it must feature strictly one
     * instance of each number in [0, rank).
     * @return std::shared_ptr<array> Permuted layout.
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
     * @return std::shared_ptr<array> Permuted layout.
     * @throws std::invalid_argument If either axis1 or axis2 exceeds bounds.
     */
    XMIPP4_NODISCARD
    array swap_axes(std::ptrdiff_t axis1, std::ptrdiff_t axis2);

    /**
     * @brief Remove insignificant axes from the layout.
     * 
     * @return std::shared_ptr<const array> The resulting layout.
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
     * @return std::shared_ptr<array> The resulting broadcasted layout.
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
