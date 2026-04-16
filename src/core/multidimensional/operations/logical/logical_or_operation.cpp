// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical/logical_or_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string logical_or_operation::get_name() const
{
	return "logical_or";
}

std::size_t logical_or_operation::get_output_count() const noexcept { return 1; }
std::size_t logical_or_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
