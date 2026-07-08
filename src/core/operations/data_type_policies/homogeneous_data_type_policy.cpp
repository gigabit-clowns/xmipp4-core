// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/operations/data_type_policies/homogeneous_data_type_policy.hpp>

#include <xmipp4/core/platform/assert.hpp>

#include "data_type_policy_helpers.hpp"

namespace xmipp4
{
namespace operations
{

void homogeneous_data_type_policy::deduce(
	span<numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	XMIPP4_CONST_CONSTEXPR auto context =
		"homogeneous_data_type_policy::deduce";

	if (input_types.empty())
	{
		fill(canonical_output_types, numerical_type::unknown);
		return;
	}

	const auto reference = require_same(input_types, context);
	require_valid(reference, context);
	fill(canonical_output_types, reference);
}

void homogeneous_data_type_policy::accept(
	span<const numerical_type> user_output_types,
	span<const numerical_type> canonical_output_types,
	span<const numerical_type> input_types
) const
{
	if (input_types.empty())
	{
		// The inputs did not constrain the type, so the user-supplied outputs
		// define the (homogeneous) operand type: require them to agree and be
		// valid.
		XMIPP4_CONST_CONSTEXPR auto context =
			"homogeneous_data_type_policy::accept";
		XMIPP4_ASSERT( 
			require_same(canonical_output_types, context) == 
			numerical_type::unknown 
		);
		const auto reference = require_same(user_output_types, context);
		require_valid(reference, context);
		return;
	}

	// The inputs already fixed the canonical type; require the user outputs to
	// match it exactly, as the base policy does.
	data_type_policy::accept(
		user_output_types,
		canonical_output_types,
		input_types
	);
}

const homogeneous_data_type_policy&
homogeneous_data_type_policy::get() noexcept
{
	static const homogeneous_data_type_policy instance;
	return instance;
}

} // namespace operations
} // namespace xmipp4
