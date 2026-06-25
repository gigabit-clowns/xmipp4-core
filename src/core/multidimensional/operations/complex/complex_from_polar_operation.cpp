// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/complex/complex_from_polar_operation.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/complex_from_real_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string complex_from_polar_operation::get_name() const
{
    return "complex_from_polar";
}

const operation_shape_policy& 
complex_from_polar_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy& 
complex_from_polar_operation::get_operation_data_type_policy() const noexcept
{
	return complex_from_real_operation_data_type_policy::get();
}

operation_arity complex_from_polar_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
