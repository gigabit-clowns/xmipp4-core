// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_xor_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string bitwise_xor_operation::get_name() const
{
	return "bitwise_xor";
}

std::size_t bitwise_xor_operation::get_output_count() const noexcept { return 1; }
std::size_t bitwise_xor_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
