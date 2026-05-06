// SPDX-License-Identifier: GPL-3.0-only

#include "select_operation_data_type_policy.hpp"

#include "data_type_policy_helpers.hpp"

#include <stdexcept>

namespace xmipp4
{
namespace multidimensional
{

void select_operation_data_type_policy::deduce(
    span<numerical_type> canonical_output_types,
    span<const numerical_type> input_types
) const
{
    XMIPP4_ASSERT(canonical_output_types.size() == 1);
    XMIPP4_ASSERT(input_types.size() == 3);

	XMIPP4_CONST_CONSTEXPR auto context =
    	"select_operation_data_type_policy::deduce";

    const auto mask_type  = input_types[0];
    require_exact(make_span(&mask_type, 1), numerical_type::boolean, context);

	const span<const numerical_type> value_types(
		std::next(input_types.data()),
		2
	);
    const auto reference = require_same(value_types, context);
    require_valid(reference, context);

    canonical_output_types[0] = reference;
}

const select_operation_data_type_policy& 
select_operation_data_type_policy::get() noexcept
{
    static const select_operation_data_type_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
