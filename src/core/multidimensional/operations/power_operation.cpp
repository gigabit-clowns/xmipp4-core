// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power_operation.hpp>

namespace xmipp4 
{
namespace multidimensional
{

std::string power_operation::get_name() const
{
	return "power";
}

std::string exp_operation::get_name() const
{
	return "exp";
}

std::string log_operation::get_name() const
{
	return "log";
}

std::string square_operation::get_name() const
{
	return "square";
}

std::string sqrt_operation::get_name() const
{
	return "sqrt";
}

} // namespace multidimensional
} // namespace xmipp4
