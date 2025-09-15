// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/kernel_access_layout.hpp>

#include "kernel_access_layout_implementation.hpp"

#include <sstream>

namespace xmipp4 
{
namespace multidimensional
{

kernel_access_layout::kernel_access_layout(
    std::unique_ptr<const kernel_access_layout_implementation> implementation
)
    : m_implementation(std::move(implementation))
{
}

kernel_access_layout::kernel_access_layout(
    kernel_access_layout&& other
) noexcept = default;


kernel_access_layout::~kernel_access_layout() = default;

kernel_access_layout& 
kernel_access_layout::operator=(kernel_access_layout&& other) noexcept = default;

std::size_t kernel_access_layout::get_number_of_operands() const noexcept
{
    return m_implementation ? m_implementation->get_number_of_operands() : 0;
}

span<const std::size_t> 
kernel_access_layout::get_batch_extents() const
{
    require_implementation("get_batch_extents");
    return m_implementation->get_batch_extents();
}

span<const std::size_t> 
kernel_access_layout::get_core_extents(std::size_t operand) const
{
    require_implementation("get_core_extents");
    return m_implementation->get_core_extents(operand);
}

span<const std::ptrdiff_t> 
kernel_access_layout::get_batch_strides(std::size_t operand) const
{
    require_implementation("get_batch_strides");
    return m_implementation->get_batch_strides(operand);
}

span<const std::ptrdiff_t> 
kernel_access_layout::get_core_strides(std::size_t operand) const
{
    require_implementation("get_core_strides");
    return m_implementation->get_core_strides(operand);
}

numerical_type kernel_access_layout::get_data_type(std::size_t operand) const
{
    require_implementation("get_data_type");
    return m_implementation->get_data_type(operand);
}

void kernel_access_layout::require_implementation(
    const char *function_name
) const
{
    if (!m_implementation)
    {
        std::ostringstream oss;
        oss << "Cannot call " << function_name;
        oss << " on a moved kernel_access_layout";
    }
}

} // namespace multidimensional
} // namespace xmipp4
