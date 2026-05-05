// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/real_from_complex_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void real_from_complex_operation_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	const auto input_type = check_inputs(
		input_types,
		"real_from_complex_operation_data_type_policy::deduce"
	);
	fill(canonical_output_types, make_real(input_type));
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
