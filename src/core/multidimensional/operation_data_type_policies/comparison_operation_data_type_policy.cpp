// SPDX-License-Identifier: GPL-3.0-only

#include "comparison_operation_data_type_policy.hpp"

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace multidimensional
{

void comparison_operation_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	XMIPP4_ASSERT(canonical_output_types.size() == 1);
	XMIPP4_ASSERT(input_types.size() == 2);

	XMIPP4_CONST_CONSTEXPR auto context = 
		"comparison_operation_data_type_policy::deduce";
	const auto reference = require_same(input_types, context);
	require_valid(reference, context);
	require_category(
		reference,
		{
			numerical_type_category::boolean,
			numerical_type_category::character,
			numerical_type_category::signed_integer,
			numerical_type_category::unsigned_integer,
			numerical_type_category::floating_point
		},
		"non-complex",
		context
	);

	canonical_output_types[0] = numerical_type::boolean;
}

const comparison_operation_data_type_policy& 
comparison_operation_data_type_policy::get() noexcept
{
	static const comparison_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
