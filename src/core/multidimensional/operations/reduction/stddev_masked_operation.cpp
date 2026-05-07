// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/stddev_masked_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

stddev_masked_operation::stddev_masked_operation(
    std::vector<std::size_t> axes,
    std::size_t ddof,
    bool keep_dim
)
    : masked_reduction_operation(std::move(axes), keep_dim)
    , m_ddof(ddof)
{
}

std::string stddev_masked_operation::get_name() const
{
    return "stddev_masked";
}
std::size_t stddev_masked_operation::get_output_count() const noexcept
{
    return 1;
}
std::size_t stddev_masked_operation::get_input_count() const noexcept
{
    return 2;
}
std::size_t stddev_masked_operation::get_degrees_of_freedom() const noexcept
{
    return m_ddof;
}

} // namespace multidimensional
} // namespace xmipp4
