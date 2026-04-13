// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

std::string negate_operation::get_name() const
{
	return "negate";
}

std::string add_operation::get_name() const
{
	return "add";
}

std::string subtract_operation::get_name() const
{
	return "subtract";
}

std::string multiply_operation::get_name() const
{
	return "multiply";
}

std::string divide_operation::get_name() const
{
	return "divide";
}

} // namespace multidimensional
} // namespace xmipp4
