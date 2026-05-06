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
	XMIPP4_CONST_CONSTEXPR auto context =
		"real_from_complex_operation_data_type_policy::deduce";
	const auto reference = require_same(input_types, context);
	require_category(
		reference,
		{ numerical_type_category::complex },
		"complex",
		context
	);
	fill(canonical_output_types, make_real(reference));
}

const real_from_complex_operation_data_type_policy&
real_from_complex_operation_data_type_policy::get() noexcept
{
	static const real_from_complex_operation_data_type_policy instance;
	return instance;
}

} // namespace multidimensional
} // namespace xmipp4
