// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array_access_layout_operand.hpp"

#include <xmipp4/core/multidimensional/broadcast_error.hpp>

#include <vector>
#include <numeric>
#include <sstream>

/**
 * Some of the algorithms and data structured featured in this code are based 
 * on:
 * https://github.com/pytorch/pytorch/blob/main/aten/src/ATen/TensorIterator.cpp
 * 
 */

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout_implementation
{
public:
    array_access_layout_implementation() = default;
    explicit array_access_layout_implementation(
        std::vector<std::size_t> extents
    );

    void add_operand(
        std::vector<std::size_t> extents,
        std::vector<std::ptrdiff_t> strides,
        std::ptrdiff_t offset, 
        numerical_type data_type
    );

    void sort_axes_by_locality();

    void coalesce_contiguous_axes();

    std::size_t get_number_of_operands() const noexcept;

    span<const std::size_t> get_extents() const noexcept;
    
    span<const std::ptrdiff_t> get_strides(std::size_t operand) const;

    std::ptrdiff_t get_offset(std::size_t operand) const;

    numerical_type get_data_type(std::size_t operand) const;

private:
    std::vector<std::size_t> m_extents;
    std::vector<array_access_layout_operand> m_operands;

    int compare_strides(std::size_t i, std::size_t j) noexcept;

    void swap_axes(std::size_t i, std::size_t j) noexcept;

    void permute_axes(std::vector<std::size_t> permutation);

    bool try_coalesce_axes(std::size_t i, std::size_t j);

    bool can_coalesce_axes(std::size_t i, std::size_t j);

    void trim_axes(std::size_t n);

    void broadcast_operand(
        std::vector<std::size_t> &extents,
        std::vector<std::ptrdiff_t> &strides
    );

};

} // namespace multidimensional
} // namespace xmipp4

#include "array_access_layout_implementation.inl"
