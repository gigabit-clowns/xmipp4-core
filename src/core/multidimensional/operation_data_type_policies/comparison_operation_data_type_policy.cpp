// SPDX-License-Identifier: GPL-3.0-only

#include "comparison_operation_data_type_policy.hpp"

namespace xmipp4
{
namespace multidimensional
{

void comparison_operation_data_type_policy::deduce_output(
	span<numerical_type> output_types,
	span<const numerical_type>
) const
{
	XMIPP4_ASSERT(output_types.size() == 1);
	output_types[0] = numerical_type::boolean;
}

void comparison_operation_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	XMIPP4_ASSERT(output_types.size() == 1);
	XMIPP4_ASSERT(input_types.size() == 2);
	if (output_types[0] != numerical_type::boolean)
	{
		throw std::invalid_argument(
			"comparison_operation_data_type_policy::validate: expected boolean "
			"output type."
		);
	}

	if (input_types[0] != input_types[1])
	{
		throw std::invalid_argument(
			"comparison_operation_data_type_policy::validate: expected equal "
			"input operands."
		);
	}

	const auto reference = input_types[0];
	if (get_size(reference) == 0)
	{
		throw std::invalid_argument(
			"comparison_operation_data_type_policy::validate: unknown input "
			"type input operands."
		);
	}

	if (get_category(reference) == numerical_type_category::complex)
	{
		throw std::invalid_argument(
			"comparison_operation_data_type_policy::validate: can not operate "
			"on complex types."
		);
	}
}

const comparison_operation_data_type_policy& 
comparison_operation_data_type_policy::get() noexcept
{
	static const comparison_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
