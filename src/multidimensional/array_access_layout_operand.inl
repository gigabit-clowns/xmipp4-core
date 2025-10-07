// SPDX-License-Identifier: GPL-3.0-only

#include "array_access_layout_operand.hpp"

namespace xmipp4 
{
namespace multidimensional
{

inline
array_access_layout_operand::array_access_layout_operand(
    std::vector<std::size_t> extents,
    std::vector<std::ptrdiff_t> strides,
    std::ptrdiff_t offset,
    numerical_type data_type,
    std::size_t number_of_core_axes
)
    : m_extents(std::move(extents))
    , m_strides(std::move(strides))
    , m_offset(offset)
    , m_data_type(data_type)
    , m_number_of_core_axes(number_of_core_axes)
{
}

inline
span<const std::size_t> 
array_access_layout_operand::get_extents() const noexcept
{
    return make_span(m_extents);
}

inline
span<const std::ptrdiff_t> 
array_access_layout_operand::get_strides() const noexcept
{
    return make_span(m_strides);
}

inline
std::ptrdiff_t array_access_layout_operand::get_offset() const noexcept
{
    return m_offset;
}

inline
numerical_type array_access_layout_operand::get_data_type() const noexcept
{
    return m_data_type;
}

inline
std::size_t 
array_access_layout_operand::get_number_of_core_axes() const noexcept
{
    return m_number_of_core_axes;
}

inline
std::size_t 
array_access_layout_operand::get_number_of_batch_axes() const noexcept
{
    return m_extents.size() - m_number_of_core_axes;
}

inline
int array_access_layout_operand::compare_strides(
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
void array_access_layout_operand::swap_axes(
    std::size_t i, 
    std::size_t j
) noexcept
{
    std::swap(m_strides[i], m_strides[j]);
    std::swap(m_extents[i], m_extents[j]);
}

void array_access_layout_operand::trim_batch_axes(std::size_t n)
{
    const auto current_batch_axes = get_number_of_batch_axes();
    if (n >= current_batch_axes)
    {
        return;
    }

    m_extents.erase(
        std::next(m_extents.cbegin(), n), 
        std::prev(m_extents.cend(), current_batch_axes)
    );
    m_strides.erase(
        std::next(m_strides.cbegin(), n), 
        std::prev(m_strides.cend(), current_batch_axes)
    );
}

} // namespace multidimensional
} // namespace xmipp4
