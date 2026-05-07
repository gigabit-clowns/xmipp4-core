// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/variance_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

variance_operation::variance_operation(
    std::vector<std::size_t> axes,
    std::size_t ddof,
    bool keep_dim
)
    : floating_reduction_operation(std::move(axes), keep_dim)
    , m_ddof(ddof)
{
}

std::string variance_operation::get_name() const { return "variance"; }
std::size_t variance_operation::get_output_count() const noexcept { return 1; }
std::size_t variance_operation::get_input_count() const noexcept { return 1; }
std::size_t variance_operation::get_degrees_of_freedom() const noexcept
{
    return m_ddof;
}

} // namespace multidimensional
} // namespace xmipp4
