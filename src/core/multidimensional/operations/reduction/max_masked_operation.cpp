// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/max_masked_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string max_masked_operation::get_name() const { return "max_masked"; }
std::size_t max_masked_operation::get_output_count() const noexcept { return 1; }
std::size_t max_masked_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
