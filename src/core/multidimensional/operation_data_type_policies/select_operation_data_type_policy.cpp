// SPDX-License-Identifier: GPL-3.0-only

#include "select_operation_data_type_policy.hpp"

#include <stdexcept>

#include <xmipp4/core/numerical_type.hpp>

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

    if (input_types[0] != numerical_type::boolean)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::deduce: first input must be "
            "boolean."
        );
    }

    const auto reference = input_types[1];
    if (get_size(reference) == 0)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::deduce: Expected valid input "
            "type."
        );
    }

    if (input_types[2] != reference)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::deduce: second and third "
            "inputs must share the same type."
        );
    }

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
