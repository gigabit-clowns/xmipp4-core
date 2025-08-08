// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_access_layout.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <vector>
#include <numeric>

/**
 * The algorithms and data structured featured in this code are based on:
 * https://github.com/pytorch/pytorch/blob/main/aten/src/ATen/TensorIterator.cpp
 * 
 */

namespace xmipp4 
{
namespace multidimensional
{

class array_access_layout::operand
{
public:
    operand(std::vector<std::size_t> kernel_extents,
            std::vector<std::ptrdiff_t> strides,
            std::ptrdiff_t offset )
        : m_kernel_extents(std::move(kernel_extents))
        , m_strides(std::move(strides))
        , m_offset(offset)
    {
    }

    span<const std::size_t> get_kernel_extents() const noexcept
    {
        return make_span(m_kernel_extents);
    }

    span<const std::ptrdiff_t> get_kernel_strides() const noexcept
    {
        return span<const std::ptrdiff_t>(
            m_strides.data() + get_number_of_batch_axes(),
            get_number_of_kernel_axes()
        );
    }

    span<const std::ptrdiff_t> get_batch_strides() const noexcept
    {
        return span<const std::ptrdiff_t>(
            m_strides.data(),
            get_number_of_batch_axes()
        );
    }

    bool get_stride(std::size_t index) const noexcept
    {
        return m_strides[index];
    }

    int compare_batch_strides(std::size_t i, std::size_t j) const noexcept
    {
        const auto stride_i = m_strides[i];
        const auto stride_j = m_strides[j];

        if (stride_i == 0 || stride_j == 0)
        {
            return 0;
        }
        else
        {
            const auto cmp_gt = static_cast<int>(stride_i > stride_j);
            const auto cmp_lt = static_cast<int>(stride_i < stride_j);
            return cmp_gt - cmp_lt;
        }
    }

    void swap_batch_axes(std::size_t i, std::size_t j) noexcept
    {
        std::swap(m_strides[i], m_strides[j]);
    }

    void trim_batch_axes(std::size_t n)
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

private:
    std::vector<std::size_t> m_kernel_extents;
    std::vector<std::ptrdiff_t> m_strides;
    std::ptrdiff_t m_offset;
 
    std::size_t get_number_of_kernel_axes() const noexcept
    {
        return m_kernel_extents.size();
    }

    std::size_t get_number_of_batch_axes() const noexcept
    {
        return m_strides.size() - m_kernel_extents.size();
    }

};

class array_access_layout::implementation
{
public:
    implementation(std::vector<std::size_t> batch_extents)
        : m_batch_extents(std::move(batch_extents))
    {
    }

    void add_operand(std::vector<std::size_t> extents,
                     std::vector<std::ptrdiff_t> strides,
                     std::ptrdiff_t offset, 
                     std::size_t kernel_dimensions )
    {
        XMIPP4_ASSERT( extents.size() == strides.size() );
        XMIPP4_ASSERT( kernel_dimensions <= extents.size() );

        const auto batch_extents_end = 
            std::prev(extents.cend(), kernel_dimensions);
        const auto equal_batch_extents = std::equal(
            m_batch_extents.cbegin(), m_batch_extents.cend(), 
            extents.cbegin(), batch_extents_end
        );
        if (!equal_batch_extents)
        {
            throw std::invalid_argument(
                "provided batch size does not match with the configured "
                "batch size"
            );
        }

        extents.erase(extents.cbegin(), batch_extents_end);
        m_operands.emplace_back(
            std::move(extents),
            std::move(strides),
            offset
        );
    }

    void optimize_batch()
    {
        sort_batch_axes_by_locality();
        coalesce_contiguous_batch_axes();
    }

    std::size_t get_number_of_operands() const noexcept
    {
        return m_operands.size();
    }

    span<const std::size_t> get_batch_extents() const noexcept
    {
        return make_span(m_batch_extents);
    }
    
    span<const std::size_t> get_kernel_extents(std::size_t operand) const
    {
        return m_operands.at(operand).get_kernel_extents();
    }

    span<const std::ptrdiff_t> get_batch_strides(std::size_t operand) const
    {
        return m_operands.at(operand).get_batch_strides();
    }

    span<const std::ptrdiff_t> get_kernel_strides(std::size_t operand) const
    {
        return m_operands.at(operand).get_kernel_strides();
    }

private:
    std::vector<std::size_t> m_batch_extents;
    std::vector<operand> m_operands;

    std::size_t get_number_of_batch_axes() const noexcept
    {
        return m_batch_extents.size();
    }

    int compare_batch_strides(std::size_t i, std::size_t j) noexcept
    {
        for (const auto &operand : m_operands)
        {
            const auto cmp = operand.compare_batch_strides(i, j);
            if (cmp != 0)
            {
                return cmp;
            }
        }

        return 0;
    }

    void swap_batch_axes(std::size_t i, std::size_t j) noexcept
    {
        std::swap(m_batch_extents[i], m_batch_extents[j]);
        for (auto &operand : m_operands)
        {
            operand.swap_batch_axes(i, j);
        }
    }

    void permute_batch_axes(std::vector<std::size_t> permutation)
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

    bool can_coalesce_batch_axes(std::size_t i, std::size_t j)
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

    void trim_batch_axes(std::size_t n)
    {
        m_batch_extents.resize(n);
        for (auto &operand : m_operands)
        {
            operand.trim_batch_axes(n);
        }
    }

    void sort_batch_axes_by_locality()
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

    void coalesce_contiguous_batch_axes()
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

};





array_access_layout::array_access_layout() noexcept = default;

array_access_layout::array_access_layout(std::vector<std::size_t> batch_extents)
    : m_implementation(
        std::make_unique<implementation>(std::move(batch_extents))
    )
{
}

array_access_layout::array_access_layout(array_access_layout&& other) noexcept = default;

array_access_layout::~array_access_layout() = default;

array_access_layout& 
array_access_layout::operator=(array_access_layout&& other) noexcept = default;

void array_access_layout::add_operand(std::vector<std::size_t> extents,
                                      std::vector<std::ptrdiff_t> strides,
                                      std::ptrdiff_t offset, 
                                      std::size_t kernel_dimensions )
{
    if (extents.size() != strides.size())
    {
        throw std::invalid_argument(
            "strides and extents must have the same length"
        );
    }

    if (kernel_dimensions > extents.size())
    {
        throw std::invalid_argument(
            "kernel_dimensions is out of bounds"
        );
    }

    if (!m_implementation)
    {
        std::vector<std::size_t> batch_extents(
            extents.cbegin(), 
            std::prev(extents.cend(), kernel_dimensions)
        );

        m_implementation = std::make_unique<implementation>(
            std::move(batch_extents)
        );
    }

    XMIPP4_ASSERT( m_implementation );
    m_implementation->add_operand(
        std::move(extents),
        std::move(strides),
        offset,
        kernel_dimensions
    );
}

void array_access_layout::optimize_batch()
{
    if (m_implementation)
    {
        m_implementation->optimize_batch();
    }
}

std::size_t array_access_layout::get_number_of_operands() const noexcept
{
    return m_implementation ? m_implementation->get_number_of_operands() : 0;
}

span<const std::size_t> 
array_access_layout::get_batch_extents() const
{
    if (!m_implementation)
    {
        throw std::logic_error(
            "Cannot call get_batch_extents on an uninitialized "
            "array_access_layout"
        );
    }

    return m_implementation->get_batch_extents();
}

span<const std::size_t> 
array_access_layout::get_kernel_extents(std::size_t operand) const
{
    if (!m_implementation)
    {
        throw std::logic_error(
            "Cannot call get_kernel_extents on an uninitialized "
            "array_access_layout"
        );
    }

    return m_implementation->get_kernel_extents(operand);
}

span<const std::ptrdiff_t> 
array_access_layout::get_batch_strides(std::size_t operand) const
{
    if (!m_implementation)
    {
        throw std::logic_error(
            "Cannot call get_batch_strides on an uninitialized "
            "array_access_layout"
        );
    }

    return m_implementation->get_batch_strides(operand);
}

span<const std::ptrdiff_t> 
array_access_layout::get_kernel_strides(std::size_t operand) const
{
    if (!m_implementation)
    {
        throw std::logic_error(
            "Cannot call get_kernel_strides on an uninitialized "
            "array_access_layout"
        );
    }

    return m_implementation->get_kernel_strides(operand);
}

} // namespace multidimensional
} // namespace xmipp4
