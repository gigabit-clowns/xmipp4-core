// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/numerical_type.hpp>
#include <xmipp4/core/span.hpp>

#include <vector>

namespace xmipp4 
{
namespace multidimensional
{

class kernel_access_layout_operand
{
public:
    kernel_access_layout_operand(std::vector<std::size_t> kernel_extents,
                                 std::vector<std::ptrdiff_t> strides,
                                 std::ptrdiff_t offset,
                                 numerical_type data_type )
        : m_core_extents(std::move(kernel_extents))
        , m_strides(std::move(strides))
        , m_offset(offset)
        , m_data_type(data_type)
    {
    }

    span<const std::size_t> get_core_extents() const noexcept
    {
        return make_span(m_core_extents);
    }

    span<const std::ptrdiff_t> get_core_strides() const noexcept
    {
        return span<const std::ptrdiff_t>(
            m_strides.data() + get_number_of_batch_axes(),
            get_number_of_core_axes()
        );
    }

    span<const std::ptrdiff_t> get_batch_strides() const noexcept
    {
        return span<const std::ptrdiff_t>(
            m_strides.data(),
            get_number_of_batch_axes()
        );
    }

    numerical_type get_data_type() const noexcept
    {
        return m_data_type;
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
            return stride_i - stride_j;
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
    std::vector<std::size_t> m_core_extents;
    std::vector<std::ptrdiff_t> m_strides;
    std::ptrdiff_t m_offset;
    numerical_type m_data_type;
 
    std::size_t get_number_of_core_axes() const noexcept
    {
        return m_core_extents.size();
    }

    std::size_t get_number_of_batch_axes() const noexcept
    {
        return m_strides.size() - m_core_extents.size();
    }

};

} // namespace multidimensional
} // namespace xmipp4
