// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/numeric/minimum_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string minimum_operation::get_name() const
{
    return "minimum";
}

std::size_t minimum_operation::get_output_count() const noexcept { return 1; }
std::size_t minimum_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
