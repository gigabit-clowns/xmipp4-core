// SPDX-License-Identifier: GPL-3.0-only

#include "select_operation_data_type_policy.hpp"

#include <stdexcept>

#include <xmipp4/core/numerical_type.hpp>

namespace xmipp4
{
namespace multidimensional
{

void select_operation_data_type_policy::deduce_output(
    span<numerical_type> output_types,
    span<const numerical_type> input_types
) const
{
	XMIPP4_ASSERT(output_types.size() == 1);
	XMIPP4_ASSERT(input_types.size() == 3);
	if (get_size(input_types[1]) == 0)
	{
		throw std::invalid_argument(
			"select_operation_data_type_policy::deduce_output: Expected valid " 
			"input type."
		);
	}
	output_types[0] = input_types[1];
}

void select_operation_data_type_policy::validate(
    span<const numerical_type> output_types,
    span<const numerical_type> input_types
) const
{
	XMIPP4_ASSERT(output_types.size() == 1);
	XMIPP4_ASSERT(input_types.size() == 3);

    if (input_types[0] != numerical_type::boolean)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: first input must be "
            "boolean."
        );
    }

    const auto reference_type = output_types[0];
    if (get_size(reference_type) == 0)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: unknown output type."
        );
    }

    if (input_types[1] != reference_type)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: second input must "
			"match the output type."
        );
    }

    if (input_types[2] != reference_type)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: third input must "
			"match the output type."
        );
    }
}

const select_operation_data_type_policy& 
select_operation_data_type_policy::get() noexcept
{
    static const select_operation_data_type_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
