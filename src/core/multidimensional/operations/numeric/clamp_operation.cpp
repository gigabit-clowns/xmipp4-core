// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/numeric/clamp_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string clamp_operation::get_name() const
{
    return "clamp";
}

std::size_t clamp_operation::get_output_count() const noexcept { return 1; }
std::size_t clamp_operation::get_input_count() const noexcept { return 3; }

} // namespace multidimensional
} // namespace xmipp4
