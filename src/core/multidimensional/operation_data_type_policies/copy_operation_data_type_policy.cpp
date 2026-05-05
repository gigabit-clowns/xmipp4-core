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
	XMIPP4_ASSERT(output_types.size() == 1);
	XMIPP4_ASSERT(input_types.size() == 1);

	if (get_size(input_types[0]) == 0)
	{
		throw std::invalid_argument(
			"copy_operation_data_type_policy::deduce_output: Expected valid " 
			"input type."
		);
	}
	output_types[0] = input_types[0];
}

void copy_operation_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	XMIPP4_ASSERT(output_types.size() == 1);
	XMIPP4_ASSERT(input_types.size() == 1);
	if (get_size(output_types[0]) == 0)
	{
		throw std::invalid_argument(
			"copy_operation_data_type_policy::validate: Expected valid " 
			"output type"
		);
	}
	if (get_size(input_types[0]) == 0)
	{
		throw std::invalid_argument(
			"copy_operation_data_type_policy::validate: Expected valid " 
			"input type"
		);
	}
}

const copy_operation_data_type_policy& 
copy_operation_data_type_policy::get() noexcept
{
	static const copy_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
