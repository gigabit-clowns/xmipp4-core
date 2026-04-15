// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/compare_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void comparison_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	// TODO
}

void comparison_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	// TODO
}

const comparison_data_type_policy& comparison_data_type_policy::get() noexcept
{
	static const comparison_data_type_policy instance;
	return instance;
}

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

std::string less_equal_operation::get_name() const
{
	return "less_equal";
}

std::string equal_operation::get_name() const
{
	return "equal";
}

std::string inequal_operation::get_name() const
{
	return "inequal";
}

} // namespace multidimensional
} // namespace xmipp4
