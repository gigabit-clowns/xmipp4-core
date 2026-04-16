// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical_operation.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/data_type_policies/boolean_data_type_policy.hpp>

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
	return boolean_data_type_policy::get();
}

std::string logical_negate_operation::get_name() const
{
	return "logical_negate";
}

std::size_t 
logical_negate_operation::get_output_count() const noexcept { return 1; }
std::size_t 
logical_negate_operation::get_input_count() const noexcept { return 1; }

std::string logical_and_operation::get_name() const
{
	return "logical_and";
}

std::size_t 
logical_and_operation::get_output_count() const noexcept { return 1; }
std::size_t 
logical_and_operation::get_input_count() const noexcept { return 2; }

std::string logical_or_operation::get_name() const
{
	return "logical_or";
}

std::size_t 
logical_or_operation::get_output_count() const noexcept { return 1; }
std::size_t 
logical_or_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
