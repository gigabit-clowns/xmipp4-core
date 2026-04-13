// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

std::string bitwise_negate_operation::get_name() const
{
	return "bitwise_negate";
}

std::string bitwise_and_operation::get_name() const
{
	return "bitwise_and";
}

std::string bitwise_or_operation::get_name() const
{
	return "bitwise_or";
}

std::string bitwise_xor_operation::get_name() const
{
	return "bitwise_xor";
}

} // namespace multidimensional
} // namespace xmipp4
