// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic/abs_operation.hpp>

#include <core/multidimensional/operation_data_type_policies/abs_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string abs_operation::get_name() const
{
	return "abs";
}

std::size_t abs_operation::get_output_count() const noexcept { return 1; }
std::size_t abs_operation::get_input_count() const noexcept { return 1; }

const operation_data_type_policy& 
abs_operation::get_operation_data_type_policy() const noexcept
{
	return abs_operation_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
