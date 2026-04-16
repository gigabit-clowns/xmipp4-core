// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/policies/boolean_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void boolean_data_type_policy::infer_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	detail::require_exact(
		input_types,
		numerical_type::boolean,
		"boolean_data_type_policy::infer_output"
	);
	detail::fill(output_types, numerical_type::boolean);
}

void boolean_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	detail::require_exact(
		output_types,
		numerical_type::boolean,
		"boolean_data_type_policy::validate"
	);
	detail::require_exact(
		input_types,
		numerical_type::boolean,
		"boolean_data_type_policy::validate"
	);
}

const boolean_data_type_policy& boolean_data_type_policy::get() noexcept
{
	static const boolean_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
