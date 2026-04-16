// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/compare_operation.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <core/multidimensional/data_type_policies/comparison_data_type_policy.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{
const shape_policy& compare_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& compare_operation::get_data_type_policy() const noexcept
{
	return comparison_data_type_policy::get();
}

std::string less_operation::get_name() const
{
	return "less";
}

std::size_t less_operation::get_output_count() const noexcept { return 1; }
std::size_t less_operation::get_input_count() const noexcept { return 2; }

std::string less_equal_operation::get_name() const
{
	return "less_equal";
}

std::size_t 
less_equal_operation::get_output_count() const noexcept { return 1; }
std::size_t 
less_equal_operation::get_input_count() const noexcept { return 2; }

std::string equal_operation::get_name() const
{
	return "equal";
}

std::size_t equal_operation::get_output_count() const noexcept { return 1; }
std::size_t equal_operation::get_input_count() const noexcept { return 2; }

std::string inequal_operation::get_name() const
{
	return "inequal";
}

std::size_t inequal_operation::get_output_count() const noexcept { return 1; }
std::size_t inequal_operation::get_input_count() const noexcept { return 2; }

} // namespace multidimensional
} // namespace xmipp4
