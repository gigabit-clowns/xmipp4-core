// SPDX-License-Identifier: GPL-3.0-only

#include "kernel_access_layout_operand.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
kernel_access_layout_operand::kernel_access_layout_operand(
    std::vector<std::size_t> kernel_extents,
    std::vector<std::ptrdiff_t> strides,
    std::ptrdiff_t offset,
    numerical_type data_type 
)
    : m_core_extents(std::move(kernel_extents))
    , m_strides(std::move(strides))
    , m_offset(offset)
    , m_data_type(data_type)
{
}

inline
span<const std::size_t> 
kernel_access_layout_operand::get_core_extents() const noexcept
{
    return make_span(m_core_extents);
}

inline
span<const std::ptrdiff_t> 
kernel_access_layout_operand::get_core_strides() const noexcept
{
    return span<const std::ptrdiff_t>(
        m_strides.data() + get_number_of_batch_axes(),
        get_number_of_core_axes()
    );
}

inline
span<const std::ptrdiff_t> 
kernel_access_layout_operand::get_batch_strides() const noexcept
{
    return span<const std::ptrdiff_t>(
        m_strides.data(),
        get_number_of_batch_axes()
    );
}

inline
std::ptrdiff_t kernel_access_layout_operand::get_offset() const noexcept
{
    return m_offset;
}

inline
numerical_type kernel_access_layout_operand::get_data_type() const noexcept
{
    return m_data_type;
}

inline
bool kernel_access_layout_operand::get_stride(std::size_t index) const noexcept
{
    return m_strides[index];
}

inline
int kernel_access_layout_operand::compare_batch_strides(
    std::size_t i, 
    std::size_t j
) const noexcept
{
    const auto stride_i = m_strides[i];
    const auto stride_j = m_strides[j];

    if (stride_i == 0 || stride_j == 0)
    {
        return 0;
    }
    else
    {
        return stride_i - stride_j;
    }
}

inline
void kernel_access_layout_operand::swap_batch_axes(
    std::size_t i, 
    std::size_t j
) noexcept
{
    std::swap(m_strides[i], m_strides[j]);
}

void kernel_access_layout_operand::trim_batch_axes(std::size_t n)
{
    const auto prev = get_number_of_batch_axes();
    if (n < prev)
    {
        m_strides.erase(
            std::next(m_strides.cbegin(), n),
            std::next(m_strides.cbegin(), prev)
        );
    }
}

inline
std::size_t 
kernel_access_layout_operand::get_number_of_core_axes() const noexcept
{
    return m_core_extents.size();
}

inline
std::size_t 
kernel_access_layout_operand::get_number_of_batch_axes() const noexcept
{
    return m_strides.size() - m_core_extents.size();
}

} // namespace multidimensional
} // namespace xmipp4
