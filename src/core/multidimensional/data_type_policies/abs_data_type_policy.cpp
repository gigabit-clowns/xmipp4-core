// SPDX-License-Identifier: GPL-3.0-only

#include <core/multidimensional/data_type_policies/abs_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void abs_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto input_type = check_input(
		input_types,
		"abs_data_type_policy::infer_output"
	);
	fill(output_types, expected_output(input_type));
}

void abs_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto input_type = check_input(
		input_types,
		"abs_data_type_policy::validate"
	);
	require_exact(
		output_types,
		expected_output(input_type),
		"abs_data_type_policy::validate"
	);
}

numerical_type abs_data_type_policy::check_input(
	span<const numerical_type> input_types,
	const char *context
)
{
	const auto reference = require_same(input_types, context);
	switch (get_category(reference))
	{
	case numerical_type_category::signed_integer:
	case numerical_type_category::unsigned_integer:
	case numerical_type_category::floating_point:
	case numerical_type_category::complex:
		break;
	default:
		throw_category(reference, "arithmetic", context);
	}
	return reference;
}

numerical_type 
abs_data_type_policy::expected_output(numerical_type input_type) noexcept
{
	if (get_category(input_type) == numerical_type_category::complex)
	{
		return make_real(input_type);
	}

	return input_type;
}

const abs_data_type_policy& abs_data_type_policy::get() noexcept
{
	static const abs_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
