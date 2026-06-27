// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/complex/complex_angle_operation.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operation_data_type_policies/real_from_complex_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string complex_angle_operation::get_name() const
{
    return "complex_angle";
}

const operation_shape_policy& 
complex_angle_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy& 
complex_angle_operation::get_operation_data_type_policy() const noexcept
{
	return real_from_complex_operation_data_type_policy::get();
}

operation_arity complex_angle_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
