// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/trigonometric_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operations/policies/same_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const shape_policy& trigonometric_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& 
trigonometric_operation::get_data_type_policy() const noexcept
{
	return same_floating_data_type_policy::get();
}

std::string sin_operation::get_name() const
{
	return "sin";
}

std::string cos_operation::get_name() const
{
	return "cos";
}

std::string sincos_operation::get_name() const
{
	return "sincos";
}

std::string tan_operation::get_name() const
{
	return "tan";
}

std::string asin_operation::get_name() const
{
	return "asin";
}

std::string acos_operation::get_name() const
{
	return "acos";
}

std::string atan_operation::get_name() const
{
	return "atan";
}

std::string atan2_operation::get_name() const
{
	return "atan2";
}

} // namespace multidimensional
} // namespace xmipp4
