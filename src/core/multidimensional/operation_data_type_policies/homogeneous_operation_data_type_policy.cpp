// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void homogeneous_operation_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	XMIPP4_CONST_CONSTEXPR auto context =
		"homogeneous_operation_data_type_policy::deduce";
	const auto reference = require_same(input_types, context);
	require_valid(reference, context);
	fill(canonical_output_types, reference);
}

const homogeneous_operation_data_type_policy& 
homogeneous_operation_data_type_policy::get() noexcept
{
	static const homogeneous_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
