// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>

#include <vector>
#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout_operand
{
public:
    array_access_layout_operand(
        std::vector<std::ptrdiff_t> strides,
        std::ptrdiff_t offset,
        numerical_type data_type
    );

    span<const std::ptrdiff_t> get_strides() const noexcept;

    std::ptrdiff_t get_offset() const noexcept;

    numerical_type get_data_type() const noexcept;

    int compare_strides(std::size_t i, std::size_t j) const noexcept;

    void swap_axes(std::size_t i, std::size_t j) noexcept;

    void trim_batch_axes(std::size_t n);

private:
    std::vector<std::ptrdiff_t> m_strides;
    std::ptrdiff_t m_offset;
    numerical_type m_data_type;

};

} // namespace multidimensional
} // namespace xmipp4

#include "array_access_layout_operand.inl"
