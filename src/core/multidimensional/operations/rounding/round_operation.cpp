// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/rounding/round_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string round_operation::get_name() const
{
    return "round";
}

std::size_t round_operation::get_output_count() const noexcept { return 1; }
std::size_t round_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
