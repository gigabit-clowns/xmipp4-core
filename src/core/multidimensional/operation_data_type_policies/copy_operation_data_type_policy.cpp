// SPDX-License-Identifier: GPL-3.0-only

#include "copy_operation_data_type_policy.hpp"

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void copy_operation_data_type_policy::deduce_output(
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

const copy_operation_data_type_policy& copy_operation_data_type_policy::get() noexcept
{
	static const copy_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
