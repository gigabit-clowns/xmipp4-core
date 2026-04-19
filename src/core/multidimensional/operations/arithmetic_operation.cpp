// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic_operation.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_arithmetic_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const operation_shape_policy& arithmetic_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy&
arithmetic_operation::get_operation_data_type_policy() const noexcept
{
	return homogeneous_arithmetic_operation_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
