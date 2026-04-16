// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_and_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string bitwise_and_operation::get_name() const
{
	return "bitwise_and";
}

std::size_t bitwise_and_operation::get_output_count() const noexcept { return 1; }
std::size_t bitwise_and_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
