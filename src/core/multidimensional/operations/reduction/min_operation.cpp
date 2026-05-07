// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/min_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string min_operation::get_name() const { return "min"; }
std::size_t min_operation::get_output_count() const noexcept { return 1; }
std::size_t min_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
