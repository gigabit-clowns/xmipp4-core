// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_iteration_layout
{
public:
    kernel_iteration_layout() noexcept;
    explicit kernel_iteration_layout(std::vector<std::size_t> batch_extents);
    kernel_iteration_layout(const kernel_iteration_layout&) = delete;
    kernel_iteration_layout(kernel_iteration_layout&& other) noexcept;
    ~kernel_iteration_layout();

    kernel_iteration_layout& 
    operator=(const kernel_iteration_layout&) = delete;
    kernel_iteration_layout& 
    operator=(kernel_iteration_layout&& other) noexcept;

    void add_operand(std::vector<std::size_t> extents,
                     std::vector<std::ptrdiff_t> strides,
                     std::ptrdiff_t offset, 
                     std::size_t kernel_dimensions );
    
    void optimize_batch();

    std::size_t get_number_of_operands() const noexcept;
    span<const std::size_t> get_batch_extents() const;
    span<const std::size_t> get_core_extents(std::size_t operand) const;
    span<const std::ptrdiff_t> get_batch_strides(std::size_t operand) const;
    span<const std::ptrdiff_t> get_core_strides(std::size_t operand) const;

private:
    class operand;
    class implementation;
    std::unique_ptr<implementation> m_implementation;

};

} // namespace multidimensional
} // namespace xmipp4


