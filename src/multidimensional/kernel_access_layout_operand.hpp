// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_access_layout_operand
{
public:
    kernel_access_layout_operand(
        std::vector<std::size_t> kernel_extents,
        std::vector<std::ptrdiff_t> strides,
        std::ptrdiff_t offset,
        numerical_type data_type 
    );

    span<const std::size_t> get_core_extents() const noexcept;

    span<const std::ptrdiff_t> get_core_strides() const noexcept;

    span<const std::ptrdiff_t> get_batch_strides() const noexcept;

    std::ptrdiff_t get_offset() const noexcept;

    numerical_type get_data_type() const noexcept;

    bool get_stride(std::size_t index) const noexcept;

    int compare_batch_strides(std::size_t i, std::size_t j) const noexcept;

    void swap_batch_axes(std::size_t i, std::size_t j) noexcept;

    void trim_batch_axes(std::size_t n);

private:
    std::vector<std::size_t> m_core_extents;
    std::vector<std::ptrdiff_t> m_strides;
    std::ptrdiff_t m_offset;
    numerical_type m_data_type;
 
    std::size_t get_number_of_core_axes() const noexcept;

    std::size_t get_number_of_batch_axes() const noexcept;

};

} // namespace multidimensional
} // namespace xmipp4

#include "kernel_access_layout_operand.inl"
