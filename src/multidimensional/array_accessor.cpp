// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_accessor.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/multidimensional/storage.hpp>

#include <vector>
#include <numeric>

/**
 * The algorithms and data structured featured in this code are mostly based on:
 * https://github.com/pytorch/pytorch/blob/main/aten/src/ATen/TensorIterator.cpp
 * 
 */

namespace xmipp4 
{
namespace multidimensional
{

class accessor_operand
{
public:
    accessor_operand(const strided_layout &layout)
    {
        layout.get_strides(m_strides);
        m_offset = layout.get_offset();
    } 

    void set_storage(std::shared_ptr<storage> storage) noexcept
    {
        m_storage = std::move(storage);
    }

    storage* get_storage() const noexcept
    {
        return m_storage.get();
    }

    int compare_strides(std::size_t i, std::size_t j) const noexcept
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

    void swap(std::size_t i, std::size_t j) noexcept
    {
        std::swap(m_strides[i], m_strides[j]);
    }

    bool get_stride(std::size_t index) const noexcept
    {
        return m_strides[index];
    }

    void resize(std::size_t n)
    {
        m_strides.resize(n);
    }

private:
    std::vector<std::ptrdiff_t> m_strides;
    std::ptrdiff_t m_offset;
    std::shared_ptr<storage> m_storage;

};

class array_accessor::implementation
{
public:
    implementation(std::vector<std::size_t> extents)
        : m_extents(std::move(extents))
        , m_operands()
        , m_number_of_outputs(0)
    {
    }

    void add_output(const strided_layout &layout)
    {
        if (m_number_of_outputs != m_operands.size())
        {
            throw std::logic_error(
                "Cannot add outputs after an input has been added"
            );
        }
        add_operand(layout);
        ++m_number_of_outputs;
    }

    void add_input(const strided_layout &layout)
    {
        add_operand(layout.broadcast_to(make_span(m_extents)));
    }

    void set_storage(std::size_t index, std::shared_ptr<storage> storage)
    {
        m_operands.at(index).set_storage(std::move(storage));
    }

    storage* get_storage(std::size_t index) const
    {
        return m_operands.at(index).get_storage();
    }

    std::size_t get_number_of_dimensions() const noexcept
    {
        return m_extents.size();
    }

    std::size_t get_number_of_operands() const noexcept
    {
        return m_operands.size();
    }

    std::size_t get_number_of_outputs() const noexcept
    {
        return m_number_of_outputs;
    }

    std::size_t get_number_of_inputs() const noexcept
    {
        return get_number_of_operands() - get_number_of_outputs();
    }

    void sort()
    {
        const auto n = get_number_of_dimensions();
        if (n <= 1)
        {
            return; // Trivial
        }

        std::vector<std::size_t> permutation(n);
        std::iota(permutation.rbegin(), permutation.rend(), std::size_t(0));

        // Insertion sort with support for ambiguous comparisons
        for (std::size_t i = 1; i < n; ++i)
        {
            const auto index1 = i;
            for (std::size_t j = 1; j <= i; ++j)
            {
                const auto index0 = i - j;
                const auto comparison = compare_axes(
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

        permute(std::move(permutation));
    }

    void coalesce()
    {
        const auto n = get_number_of_dimensions();

        std::size_t prev = 0;
        for (std::size_t curr = 1; curr < n;  ++curr)
        {
            if (can_coalesce(prev, curr))
            {
                if (m_extents[prev] == 1)
                {
                    swap(prev, curr);
                }
                else
                {
                    m_extents[prev] *= m_extents[curr];
                }

            }
            else
            {
                ++prev;
                if (prev != curr)
                {
                    swap(prev, curr);
                }
            }
        }

        resize(prev+1);
    }

private:
    std::vector<std::size_t> m_extents;
    std::vector<accessor_operand> m_operands;
    std::size_t m_number_of_outputs;

    void add_operand(const strided_layout &layout)
    {
        m_operands.emplace_back(layout);
    }

    int compare_axes(std::size_t i, std::size_t j) noexcept
    {
        for (const auto &operand : m_operands)
        {
            const auto cmp = operand.compare_strides(i, j);
            if (cmp != 0)
            {
                return cmp;
            }
        }

        return 0;
    }

    void swap(std::size_t i, std::size_t j) noexcept
    {
        std::swap(m_extents[i], m_extents[j]);
        for (auto &operand : m_operands)
        {
            operand.swap(i, j);
        }
    }

    void permute(std::vector<std::size_t> permutation)
    {
        // Permute the extents using cycle decomposition
        const auto n = permutation.size();
        for (size_t i = 0; i < n; ++i) {
            while (permutation[i] != i) {
                swap(i, permutation[i]);
                std::swap(permutation[i], permutation[permutation[i]]);
            }
        }
    }

    bool can_coalesce(std::size_t i, std::size_t j)
    {
        const auto extent_i = m_extents[i];
        const auto extent_j = m_extents[j];
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

    void resize(std::size_t n)
    {
        m_extents.resize(n);
        for (auto &operand : m_operands)
        {
            operand.resize(n);
        }
    }

};

} // namespace multidimensional
} // namespace xmipp4
 