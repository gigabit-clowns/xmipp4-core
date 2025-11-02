// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../span.hpp"
#include "../numerical_type.hpp"

#include <memory>

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout_implementation;

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

    std::size_t get_number_of_operands() const noexcept;
    span<const std::size_t> get_batch_extents() const;
    span<const std::size_t> get_extents(std::size_t operand) const;
    span<const std::ptrdiff_t> get_strides(std::size_t operand) const;
    std::ptrdiff_t get_offset(std::size_t operand) const;
    numerical_type get_data_type(std::size_t operand) const;

private:
    std::unique_ptr<const array_access_layout_implementation> m_implementation;

    void require_implementation(const char *function_name) const;

};

} // namespace multidimensional
} // namespace xmipp4
