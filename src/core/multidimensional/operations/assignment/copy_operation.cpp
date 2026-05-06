// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/assignment/copy_operation.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>

#include <core/multidimensional/operation_data_type_policies/copy_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string copy_operation::get_name() const
{
	return "copy";
}

const operation_shape_policy& 
copy_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy& 
copy_operation::get_operation_data_type_policy() const noexcept
{
	return copy_operation_data_type_policy::get();
}

std::size_t copy_operation::get_output_count() const noexcept { return 1; }
std::size_t copy_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
