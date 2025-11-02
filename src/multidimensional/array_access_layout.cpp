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
    return m_implementation ? m_implementation->get_number_of_operands() : 0;
}

span<const std::size_t> array_access_layout::get_batch_extents() const
{
    require_implementation("get_batch_extents");
    return m_implementation->get_batch_extents();
}

span<const std::size_t> 
array_access_layout::get_extents(std::size_t operand) const
{
    require_implementation("get_extents");
    return m_implementation->get_extents(operand);
}

span<const std::ptrdiff_t> 
array_access_layout::get_strides(std::size_t operand) const
{
    require_implementation("get_strides");
    return m_implementation->get_strides(operand);
}

std::ptrdiff_t array_access_layout::get_offset(std::size_t operand) const
{
    require_implementation("get_offset");
    return m_implementation->get_offset(operand);
}

numerical_type array_access_layout::get_data_type(std::size_t operand) const
{
    require_implementation("get_data_type");
    return m_implementation->get_data_type(operand);
}

void array_access_layout::require_implementation(
    const char *function_name
) const
{
    if (!m_implementation)
    {
        std::ostringstream oss;
        oss << "Cannot call " << function_name;
        oss << " on a moved or un-initialized array_access_layout";
    }
}

} // namespace multidimensional
} // namespace xmipp4
