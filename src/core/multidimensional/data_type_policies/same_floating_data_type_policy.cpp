// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/data_type_policies/same_floating_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void same_floating_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		input_types,
		"same_floating_data_type_policy::infer_output"
	);
	check(reference);
	fill(output_types, reference);
}

void same_floating_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		output_types,
		"same_floating_data_type_policy::validate"
	);
	check(reference);
	require_exact(
		input_types, reference, "same_floating_data_type_policy::validate"
	);
}

void same_floating_data_type_policy::check(numerical_type type)
{
	if (get_category(type) != numerical_type_category::floating_point)
	{
		throw_category(
			type, "floating point", "same_floating_data_type_policy"
		);
	}
}

const same_floating_data_type_policy&
same_floating_data_type_policy::get() noexcept
{
	static const same_floating_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
