// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/real_from_complex_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void real_from_complex_operation_data_type_policy::deduce_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto input_type = check_inputs(
		input_types,
		"real_from_complex_operation_data_type_policy::infer_output"
	);
	fill(output_types, make_real(input_type));
}

void real_from_complex_operation_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto input_type = check_inputs(
		input_types,
		"real_from_complex_operation_data_type_policy::validate"
	);

	const auto expected_output = make_real(input_type);
	require_exact(
		output_types, 
		expected_output,
		"real_from_complex_operation_data_type_policy::validate"
	);
}

numerical_type real_from_complex_operation_data_type_policy::check_inputs(
	span<const numerical_type> input_types,
	const char *context
)
{
	const auto reference = require_same(input_types, context);
	if (get_category(reference) != numerical_type_category::complex)
	{
		throw_category(reference, "complex", context);
	}

	return reference;
}

const real_from_complex_operation_data_type_policy&
real_from_complex_operation_data_type_policy::get() noexcept
{
	static const real_from_complex_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
