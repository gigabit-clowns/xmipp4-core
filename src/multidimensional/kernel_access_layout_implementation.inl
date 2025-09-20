// SPDX-License-Identifier: GPL-3.0-only

#include "kernel_access_layout_operand.hpp"

/**
 * Some of the algorithms and data structured featured in this code are based 
 * on:
 * https://github.com/pytorch/pytorch/blob/main/aten/src/ATen/TensorIterator.cpp
 * 
 */

namespace xmipp4 
{
namespace multidimensional
{

inline
kernel_access_layout_implementation::kernel_access_layout_implementation(
    std::vector<std::size_t> batch_extents
)
    : m_batch_extents(std::move(batch_extents))
{
}

inline
void kernel_access_layout_implementation::add_operand(
    std::vector<std::size_t> extents,
    std::vector<std::ptrdiff_t> strides,
    std::ptrdiff_t offset, 
    numerical_type data_type,
    std::size_t core_dimensions 
)
{
    XMIPP4_ASSERT( extents.size() == strides.size() );
    XMIPP4_ASSERT( core_dimensions <= extents.size() );

    broadcast_operand(extents, strides, core_dimensions);
    extents.erase(
        extents.cbegin(), 
        std::prev(extents.cend(), core_dimensions)
    );

    m_operands.emplace_back(
        std::move(extents),
        std::move(strides),
        offset,
        data_type
    );
}

inline
void kernel_access_layout_implementation::sort_batch_axes_by_locality()
{
    const auto n = get_number_of_batch_axes();
    if (n <= 1)
    {
        return; // Trivial
    }

    // Start with reversed indices n-1, n-2 ... 1, 0
    std::vector<std::size_t> permutation(n);
    for (std::size_t i = 0; i < n; ++i)
    {
        permutation[i] = n - i - 1;
    }

    // Insertion sort with support for ambiguous comparisons
    for (std::size_t i = 1; i < n; ++i)
    {
        const auto index1 = i;
        for (std::size_t j = 1; j <= i; ++j)
        {
            const auto index0 = i - j;
            const auto comparison = compare_batch_strides(
                permutation[index0], 
                permutation[index1]
            );

            if (comparison > 0)
            {
                std::swap(permutation[index0], permutation[index1]);
                j = 0;
            } 
            else if (comparison < 0) 
            {
                break;
            }
        }
    }

    permute_batch_axes(std::move(permutation));
}

inline
void kernel_access_layout_implementation::coalesce_contiguous_batch_axes()
{
    const auto n = get_number_of_batch_axes();
    if (n <= 1)
    {
        return; // Trivial
    }

    std::size_t prev = 0;
    for (std::size_t curr = 1; curr < n;  ++curr)
    {
        if (can_coalesce_batch_axes(prev, curr))
        {
            if (m_batch_extents[prev] == 1)
            {
                swap_batch_axes(prev, curr);
            }
            else
            {
                m_batch_extents[prev] *= m_batch_extents[curr];
            }

        }
        else
        {
            ++prev;
            if (prev != curr)
            {
                swap_batch_axes(prev, curr);
            }
        }
    }

    trim_batch_axes(prev+1);
}

inline
std::size_t 
kernel_access_layout_implementation::get_number_of_operands() const noexcept
{
    return m_operands.size();
}

inline
std::size_t 
kernel_access_layout_implementation::get_number_of_batch_axes() const noexcept
{
    return m_batch_extents.size();
}

inline
span<const std::size_t> 
kernel_access_layout_implementation::get_batch_extents() const noexcept
{
    return make_span(m_batch_extents);
}

inline
span<const std::size_t> 
kernel_access_layout_implementation::get_core_extents(std::size_t operand) const
{
    return m_operands.at(operand).get_core_extents();
}

inline
span<const std::ptrdiff_t> 
kernel_access_layout_implementation::get_batch_strides(std::size_t operand) const
{
    return m_operands.at(operand).get_batch_strides();
}

inline
span<const std::ptrdiff_t> 
kernel_access_layout_implementation::get_core_strides(std::size_t operand) const
{
    return m_operands.at(operand).get_core_strides();
}

inline
numerical_type 
kernel_access_layout_implementation::get_data_type(std::size_t operand) const
{
    return m_operands.at(operand).get_data_type();
}

inline
int kernel_access_layout_implementation::compare_batch_strides(
    std::size_t i, 
    std::size_t j
) noexcept
{
    for (const auto &operand : m_operands)
    {
        const auto cmp = operand.compare_batch_strides(i, j);
        if (cmp != 0)
        {
            return cmp;
        }
    }

    return m_batch_extents[i] - m_batch_extents[j]; // Untie with extents
}

inline
void kernel_access_layout_implementation::swap_batch_axes(
    std::size_t i, 
    std::size_t j
) noexcept
{
    std::swap(m_batch_extents[i], m_batch_extents[j]);
    for (auto &operand : m_operands)
    {
        operand.swap_batch_axes(i, j);
    }
}

inline
void kernel_access_layout_implementation::permute_batch_axes(
    std::vector<std::size_t> permutation
)
{
    // Permute the extents using cycle decomposition
    const auto n = permutation.size();
    for (size_t i = 0; i < n; ++i) {
        while (permutation[i] != i) {
            swap_batch_axes(i, permutation[i]);
            std::swap(permutation[i], permutation[permutation[i]]);
        }
    }
}

inline
bool kernel_access_layout_implementation::can_coalesce_batch_axes(
    std::size_t i, 
    std::size_t j
)
{
    const auto extent_i = m_batch_extents[i];
    const auto extent_j = m_batch_extents[j];
    if (extent_i == 1 || extent_j == 1)
    {
        return true;
    }

    for (const auto &operand : m_operands)
    {
        const auto stride_i = operand.get_stride(i);
        const auto stride_j = operand.get_stride(j);
        if (extent_i * stride_i != stride_j) 
        {
            return false;
        }
    }

    return true;
}

inline
void kernel_access_layout_implementation::trim_batch_axes(std::size_t n)
{
    m_batch_extents.resize(n);
    for (auto &operand : m_operands)
    {
        operand.trim_batch_axes(n);
    }
}

inline
void kernel_access_layout_implementation::broadcast_operand(
    std::vector<std::size_t> &extents,
    std::vector<std::ptrdiff_t> &strides,
    std::size_t core_dimensions 
)
{
    const auto batch_dimensions = get_number_of_batch_axes();

    if ((batch_dimensions + core_dimensions) < extents.size())
    {
        std::ostringstream oss;
        oss << "Operand with " << extents.size() - core_dimensions 
            << " batch dimensions exceeds the batch dimensionality "
            << batch_dimensions
            << " required in the kernel_access_layout";

        throw std::invalid_argument(oss.str());
    }

    const auto padding = 
        get_number_of_batch_axes() + core_dimensions - extents.size();
    extents.insert(extents.cbegin(), padding, 1);
    strides.insert(strides.cbegin(), padding, 0);

    XMIPP4_ASSERT( extents.size() == batch_dimensions + core_dimensions );
    for (std::size_t i = 0; i < get_number_of_batch_axes(); ++i)
    {
        if (m_batch_extents[i] != extents[i])
        {
            if (extents[i] == 1)
            {
                // Broadcast axis
                extents[i] = m_batch_extents[i];
                strides[i] = 0;
            }
            else
            {
                throw broadcast_error(
                    m_batch_extents,
                    std::vector<std::size_t>(
                        extents.cbegin(), 
                        std::prev(extents.cend(), core_dimensions)
                    )
                );
            }
        }

        XMIPP4_ASSERT( m_batch_extents[i] == extents[i] );
    }
}

} // namespace multidimensional
} // namespace xmipp4
