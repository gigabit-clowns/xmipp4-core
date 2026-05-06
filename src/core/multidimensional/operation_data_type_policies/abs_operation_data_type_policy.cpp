// SPDX-License-Identifier: GPL-3.0-only

#include <core/multidimensional/operation_data_type_policies/abs_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void abs_operation_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	XMIPP4_CONST_CONSTEXPR auto context = 
		"abs_operation_data_type_policy::deduce";
	const auto reference = require_same(input_types, context);
	require_category(
		reference,
		{
			numerical_type_category::signed_integer,
			numerical_type_category::unsigned_integer,
			numerical_type_category::floating_point,
			numerical_type_category::complex
		},
		"arithmetic",
		context
	);
	fill(canonical_output_types, make_real(reference));
}

const abs_operation_data_type_policy& 
abs_operation_data_type_policy::get() noexcept
{
	static const abs_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
