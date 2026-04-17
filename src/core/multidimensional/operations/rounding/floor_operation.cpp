// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/rounding/floor_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string floor_operation::get_name() const
{
    return "floor";
}

std::size_t floor_operation::get_output_count() const noexcept { return 1; }
std::size_t floor_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
