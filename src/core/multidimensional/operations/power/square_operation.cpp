// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power/square_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string square_operation::get_name() const
{
	return "square";
}

std::size_t square_operation::get_output_count() const noexcept { return 1; }
std::size_t square_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
