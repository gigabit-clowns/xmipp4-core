// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operations/policies/same_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const shape_policy& logical_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& logical_operation::get_data_type_policy() const noexcept
{
	return same_floating_data_type_policy::get();
}

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
