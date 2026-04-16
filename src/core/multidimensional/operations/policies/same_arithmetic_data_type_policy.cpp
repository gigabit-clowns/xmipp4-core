// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/policies/same_arithmetic_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void same_arithmetic_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = detail::require_same(
		input_types,
		"same_arithmetic_data_type_policy::infer_output"
	);
	check(reference);
	detail::fill(output_types, reference);
}

void same_arithmetic_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = detail::require_same(
		output_types,
		"same_arithmetic_data_type_policy::validate"
	);
	check(reference);
	detail::require_exact(
		input_types, reference, "same_arithmetic_data_type_policy::validate"
	);
}

void same_arithmetic_data_type_policy::check(numerical_type type)
{
	switch (get_category(type))
	{
	case numerical_type_category::signed_integer:
	case numerical_type_category::unsigned_integer:
	case numerical_type_category::floating_point:
	case numerical_type_category::complex:
		break;
	default:
		detail::throw_category(
			type, "arithmetic", "same_arithmetic_data_type_policy"
		);
	}
}

const same_arithmetic_data_type_policy&
same_arithmetic_data_type_policy::get() noexcept
{
	static const same_arithmetic_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
