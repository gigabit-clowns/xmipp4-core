// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/complex_from_real_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void complex_from_real_operation_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	const auto input_type = check_inputs(
		input_types,
		"complex_from_real_operation_data_type_policy::deduce"
	);
	fill(canonical_output_types, make_complex(input_type));
}

numerical_type complex_from_real_operation_data_type_policy::check_inputs(
	span<const numerical_type> input_types,
	const char *context
)
{
	const auto reference = require_same(input_types, context);
	if (get_category(reference) != numerical_type_category::floating_point)
	{
		throw_category(reference, "floating-point", context);
	}

	return reference;
}

const complex_from_real_operation_data_type_policy&
complex_from_real_operation_data_type_policy::get() noexcept
{
	static const complex_from_real_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
