// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/compare/equal_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string equal_operation::get_name() const
{
	return "equal";
}

std::size_t equal_operation::get_output_count() const noexcept { return 1; }
std::size_t equal_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
