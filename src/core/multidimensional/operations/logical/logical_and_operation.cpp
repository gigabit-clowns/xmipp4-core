// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical/logical_and_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string logical_and_operation::get_name() const
{
	return "logical_and";
}

std::size_t 
logical_and_operation::get_output_count() const noexcept { return 1; }
std::size_t 
logical_and_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
