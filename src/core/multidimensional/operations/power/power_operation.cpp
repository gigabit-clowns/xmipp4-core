// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power/power_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string power_operation::get_name() const
{
	return "power";
}

std::size_t power_operation::get_output_count() const noexcept { return 1; }
std::size_t power_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
