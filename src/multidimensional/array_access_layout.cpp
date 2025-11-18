// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_access_layout.hpp>

#include "array_access_layout_implementation.hpp"

#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

array_access_layout::array_access_layout() = default;

array_access_layout::array_access_layout(
    std::unique_ptr<const array_access_layout_implementation> implementation
)
    : m_implementation(std::move(implementation))
{
}

array_access_layout::array_access_layout(
    array_access_layout&& other
) noexcept = default;


array_access_layout::~array_access_layout() = default;

array_access_layout& 
array_access_layout::operator=(array_access_layout&& other) noexcept = default;

std::size_t array_access_layout::get_number_of_operands() const noexcept
{
    if (m_implementation)
    {
        return m_implementation->get_number_of_operands();
    }
    else
    {
        return 0UL;
    }
}

span<const std::size_t> array_access_layout::get_extents() const
{
    if (m_implementation)
    {
        return m_implementation->get_extents();
    }
    else
    {
        return span<const std::size_t>();
    }
}

span<const std::ptrdiff_t> 
array_access_layout::get_strides(std::size_t operand) const
{
    if (!m_implementation)
    {
        const array_access_layout_implementation implementation;
        return implementation.get_strides(operand); // throws
    }

    return m_implementation->get_strides(operand);
}

std::ptrdiff_t array_access_layout::get_offset(std::size_t operand) const
{
    if (!m_implementation)
    {
        const array_access_layout_implementation implementation;
        return implementation.get_offset(operand); // throws
    }

    return m_implementation->get_offset(operand);
}

const array_access_layout_implementation*
array_access_layout::get_implementation() const noexcept
{
    return m_implementation.get();
}

} // namespace multidimensional
} // namespace xmipp4
