// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout_implementation;

/**
 * @brief Provides joint access semantics for array-s
 * 
 * This class enables jointly iterating over multiple arrays. It stores
 * extents of the iteration space, strides for each of the operands, 
 * offset for each of the operands and the data_type of each of the operands.
 * 
 * Instances of this object must be obtained through 
 * array_access_layout_builder. Once built, it is inmutable.
 * 
 * @see array_access_layout_builder
 * 
 */
class array_access_layout
{
public:
    array_access_layout();
    explicit array_access_layout(
        std::unique_ptr<const array_access_layout_implementation> implementation
    );
    array_access_layout(const array_access_layout&) = delete;
    array_access_layout(array_access_layout&& other) noexcept;
    ~array_access_layout();

    array_access_layout& 
    operator=(const array_access_layout&) = delete;
    array_access_layout& 
    operator=(array_access_layout&& other) noexcept;

    /**
     * @brief Get the number of operands
     * 
     * @return std::size_t The number of operands.
     */
    std::size_t get_number_of_operands() const noexcept;

    /**
     * @brief Get the extents of the iteration space.
     * 
     * @return span<const std::size_t> The extents of the iteration space.
     */
    span<const std::size_t> get_extents() const;

    /**
     * @brief Get the strides for an operand.
     * 
     * @param operand The index of the operand. Must be in 
     * [0, number_of_operands())
     * @return span<const std::ptrdiff_t> The strides of the requested operand. 
     * Always has the same size as extents.
     * 
     * @throws std::out_of_range if operand index is out of range.
     */
    span<const std::ptrdiff_t> get_strides(std::size_t operand) const;

    /**
     * @brief Get the offset parameter of an operand.
     * 
     * @param operand The index of the operand. Must be in 
     * [0, number_of_operands())
     * @return std::ptrdiff_t The offset parameter of the requested operand.
     * 
     * @throws std::out_of_range if operand index is out of range.
     */
    std::ptrdiff_t get_offset(std::size_t operand) const;

    /**
     * @brief Get a pointer to the implementation.
     * 
     * @return const array_access_layout_implementation* Pointer to the
     * implementation.
     * 
     * @note This method is meant to be used for testing purposes, as the 
     * array_access_layout_implementation is not publicly available.
     */
    const array_access_layout_implementation* 
    get_implementation() const noexcept;

private:
    std::unique_ptr<const array_access_layout_implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4
