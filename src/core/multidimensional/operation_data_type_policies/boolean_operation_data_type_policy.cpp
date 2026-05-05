// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/boolean_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void boolean_operation_data_type_policy::deduce_output(
	span<numerical_type> output_types,
	span<const numerical_type>
) const
{
	fill(output_types, numerical_type::boolean);
}

void boolean_operation_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	require_exact(
		output_types,
		numerical_type::boolean,
		"boolean_operation_data_type_policy::validate"
	);
	require_exact(
		input_types,
		numerical_type::boolean,
		"boolean_operation_data_type_policy::validate"
	);
}

const boolean_operation_data_type_policy& 
boolean_operation_data_type_policy::get() noexcept
{
	static const boolean_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
