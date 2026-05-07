// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

reduction_operation::reduction_operation(
    std::vector<std::size_t> axes,
    bool keep_dim
)
    : m_shape_policy(std::move(axes), keep_dim)
{
}

reduction_operation::~reduction_operation() = default;

const operation_shape_policy&
reduction_operation::get_operation_shape_policy() const noexcept
{
    return m_shape_policy;
}

span<const std::size_t> reduction_operation::get_reduction_axes() const noexcept
{
    return m_shape_policy.get_reduction_axes();
}

bool reduction_operation::keeps_dimensions() const noexcept
{
    return m_shape_policy.keeps_dimensions();
}

} // namespace multidimensional
} // namespace xmipp4
