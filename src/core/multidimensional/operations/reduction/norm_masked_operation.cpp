// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/norm_masked_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string norm_masked_operation::get_name() const { return "norm_masked"; }
std::size_t norm_masked_operation::get_output_count() const noexcept { return 1; }
std::size_t norm_masked_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
