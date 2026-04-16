// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>

#include <xmipp4/core/multidimensional/operations/policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void copy_operation_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	if (output_types.size() != 1)
	{
		throw std::invalid_argument(
			"copy_operation_data_type_policy::infer_output requires exactly "
			"one output"
		);
	}
	if (input_types.size() != 1)
	{
		throw std::invalid_argument(
			"copy_operation_data_type_policy::infer_output requires exactly "
			"one input"
		);
	}

	output_types[0] = input_types[0];
}

void copy_operation_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	if (output_types.size() != 1)
	{
		throw std::invalid_argument(
			"copy_operation_data_type_policy::validate requires exactly one "
			"output"
		);
	}
	if (input_types.size() != 1)
	{
		throw std::invalid_argument(
			"copy_operation_data_type_policy::validate requires exactly one "
			"input"
		);
	}
}

const data_type_policy& copy_operation_data_type_policy::get() noexcept
{
	static const copy_operation_data_type_policy instance;
	return instance;
}

std::string copy_operation::get_name() const
{
	return "copy";
}

const shape_policy& copy_operation::get_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy& copy_operation::get_data_type_policy() const noexcept
{
	return copy_operation_data_type_policy::get();
}

std::size_t copy_operation::get_output_count() const noexcept { return 1; }
std::size_t copy_operation::get_input_count() const noexcept { return 1; }

} // namespace multidimensional
} // namespace xmipp4
