// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_arithmetic_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void homogeneous_arithmetic_operation_data_type_policy::deduce_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		input_types,
		"homogeneous_arithmetic_operation_data_type_policy::deduce_output"
	);
	check(reference);
	fill(output_types, reference);
}

void homogeneous_arithmetic_operation_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		output_types,
		"homogeneous_arithmetic_operation_data_type_policy::validate"
	);
	check(reference);
	require_exact(
		input_types, 
		reference, 
		"homogeneous_arithmetic_operation_data_type_policy::validate"
	);
}

void homogeneous_arithmetic_operation_data_type_policy::check(
	numerical_type type
)
{
	switch (get_category(type))
	{
	case numerical_type_category::signed_integer:
	case numerical_type_category::unsigned_integer:
	case numerical_type_category::floating_point:
	case numerical_type_category::complex:
		break;
	default:
		throw_category(
			type, 
			"arithmetic", 
			"homogeneous_arithmetic_operation_data_type_policy"
		);
	}
}

const homogeneous_arithmetic_operation_data_type_policy&
homogeneous_arithmetic_operation_data_type_policy::get() noexcept
{
	static const homogeneous_arithmetic_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
