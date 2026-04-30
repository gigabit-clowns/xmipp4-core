// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_complex_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void homogeneous_complex_operation_data_type_policy::deduce_output(
	span<numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		input_types,
		"homogeneous_complex_operation_data_type_policy::infer_output"
	);
	check(reference);
	fill(output_types, reference);
}

void homogeneous_complex_operation_data_type_policy::validate(
	span<const numerical_type> output_types,
	span<const numerical_type> input_types
) const
{
	const auto reference = require_same(
		output_types,
		"homogeneous_complex_operation_data_type_policy::validate"
	);
	check(reference);
	require_exact(
		input_types, reference, "homogeneous_complex_operation_data_type_policy::validate"
	);
}

void homogeneous_complex_operation_data_type_policy::check(numerical_type type)
{
	if (get_category(type) != numerical_type_category::complex)
	{
		throw_category(type, "complex", "homogeneous_complex_operation_data_type_policy");
	}
}

const homogeneous_complex_operation_data_type_policy&
homogeneous_complex_operation_data_type_policy::get() noexcept
{
	static const homogeneous_complex_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
