// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power/exp_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string exp_operation::get_name() const
{
	return "exp";
}

std::size_t exp_operation::get_output_count() const noexcept { return 1; }
std::size_t exp_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
