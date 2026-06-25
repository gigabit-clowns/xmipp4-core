// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/stddev_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

stddev_operation::stddev_operation(
    std::vector<std::size_t> axes,
    std::size_t ddof,
    bool keep_dim
)
    : floating_reduction_operation(std::move(axes), keep_dim)
    , m_ddof(ddof)
{
}

std::string stddev_operation::get_name() const 
{
    return "stddev";
}
operation_arity stddev_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}
std::size_t stddev_operation::get_degrees_of_freedom() const noexcept
{
    return m_ddof;
}

} // namespace multidimensional
} // namespace xmipp4
