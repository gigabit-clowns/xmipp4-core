// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operation_data_type_policies/masked_operation_data_type_policy.hpp>

#include "data_type_policy_helpers.hpp"

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void masked_operation_data_type_policy::deduce(
    span<numerical_type> canonical_output_types,
    span<const numerical_type> input_types
) const
{
	if (input_types.size() < 2)
	{
		throw std::invalid_argument(
			"masked_operation_data_type_policy::deduce requires at least two "
			"input operands"
		);
	}

	XMIPP4_CONST_CONSTEXPR auto context =
    	"masked_operation_data_type_policy::deduce";

    const auto mask_type  = input_types[0];
    require_exact(make_span(&mask_type, 1), numerical_type::boolean, context);

	const span<const numerical_type> value_types(
		std::next(input_types.data()),
		input_types.size() - 1
	);
    const auto reference = require_same(value_types, context);
    require_valid(reference, context);

	fill(canonical_output_types, reference);
}

const masked_operation_data_type_policy& 
masked_operation_data_type_policy::get() noexcept
{
    static const masked_operation_data_type_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
