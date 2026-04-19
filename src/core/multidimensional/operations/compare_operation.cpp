// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/compare_operation.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>

#include <core/multidimensional/operation_data_type_policies/comparison_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const operation_shape_policy& compare_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy& compare_operation::get_operation_data_type_policy() const noexcept
{
	return comparison_operation_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
