// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class strided_layout;

class array_access_layout
{
public:
    array_access_layout() noexcept;
    explicit array_access_layout(std::vector<std::size_t> batch_extents);
    array_access_layout(const array_access_layout&) = delete;
    array_access_layout(array_access_layout&& other) noexcept;
    ~array_access_layout();

    array_access_layout& operator=(const array_access_layout&) = delete;
    array_access_layout& operator=(array_access_layout&& other) noexcept;

    void add_operand(std::vector<std::size_t> extents,
                     std::vector<std::ptrdiff_t> strides,
                     std::ptrdiff_t offset, 
                     std::size_t kernel_dimensions );
   
    void sort_batch_axes_by_locality();
    void coalesce_contiguous_batch_axes();

    std::size_t get_number_of_operands() const noexcept;
    span<const std::size_t> get_batch_extents() const;
    span<const std::size_t> get_kernel_extents(std::size_t operand) const;
    span<const std::ptrdiff_t> get_batch_strides(std::size_t operand) const;
    span<const std::ptrdiff_t> get_kernel_strides(std::size_t operand) const;

private:
    class operand;
    class implementation;
    std::unique_ptr<implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4


