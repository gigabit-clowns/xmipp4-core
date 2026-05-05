// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/boolean_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void boolean_operation_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	require_exact(
		input_types,
		numerical_type::boolean,
		"boolean_operation_data_type_policy::deduce"
	);
	fill(canonical_output_types, numerical_type::boolean);
}

const boolean_operation_data_type_policy& 
boolean_operation_data_type_policy::get() noexcept
{
	static const boolean_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
