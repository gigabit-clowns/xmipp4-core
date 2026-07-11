// SPDX-License-Identifier: GPL-3.0-only

#include "copy_operation_data_type_policy.hpp"

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{

void copy_operation_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	XMIPP4_ASSERT(canonical_output_types.size() == 1);
	XMIPP4_ASSERT(input_types.size() == 1);

	require_valid(
		input_types[0],
		"copy_operation_data_type_policy::deduce"
	);
	canonical_output_types[0] = input_types[0];
}

void copy_operation_data_type_policy::accept(
	span<const numerical_type> user_output_types,
	span<const numerical_type> /*canonical_output_types*/,
	span<const numerical_type> /*input_types*/
) const
{
	XMIPP4_ASSERT(user_output_types.size() == 1);

	require_valid(
		user_output_types[0],
		"copy_operation_data_type_policy::accept"
	);
}

const copy_operation_data_type_policy&
copy_operation_data_type_policy::get() noexcept
{
	static const copy_operation_data_type_policy instance;
	return instance;
}

} // namespace xmipp4
