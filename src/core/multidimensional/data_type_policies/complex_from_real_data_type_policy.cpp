// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/data_type_policies/complex_from_real_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void complex_from_real_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto input_type = check_inputs(
		input_types,
		"complex_from_real_data_type_policy::infer_output"
	);
	fill(output_types, make_complex(input_type));
}

void complex_from_real_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto input_type = check_inputs(
		input_types,
		"complex_from_real_data_type_policy::validate"
	);

	const auto expected_output = make_complex(input_type);
	require_exact(
		output_types, 
		expected_output,
		"complex_from_real_data_type_policy::validate"
	);
}

numerical_type complex_from_real_data_type_policy::check_inputs(
	span<const numerical_type> input_types,
	const char *context
)
{
	const auto reference = require_same(input_types, context);
	if (get_category(reference) != numerical_type_category::floating_point)
	{
		throw_category(reference, "real point", context);
	}

	return reference;
}

const complex_from_real_data_type_policy&
complex_from_real_data_type_policy::get() noexcept
{
	static const complex_from_real_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
