// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operations/policies/same_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const shape_policy& arithmetic_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& arithmetic_operation::get_data_type_policy() const noexcept
{
	return same_arithmetic_data_type_policy::get();
}

std::string negate_operation::get_name() const
{
	return "negate";
}

std::string conjugate_operation::get_name() const
{
	return "conjugate";
}

const shape_policy& conjugate_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& conjugate_operation::get_data_type_policy() const noexcept
{
	return same_complex_data_type_policy::get();
}

std::string abs_operation::get_name() const
{
	return "abs";
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
