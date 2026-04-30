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
    if (output_types.size() != 1)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::infer_output: expected 1 output operand."
        );
    }
    if (input_types.size() != 3)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::infer_output: expected 3 input operands "
            "(mask, x, y)."
        );
    }

    if (input_types[0] != numerical_type::boolean)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::infer_output: mask (input[0]) must be "
            "boolean."
        );
    }

    if (input_types[1] != input_types[2])
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::infer_output: x (input[1]) and y "
            "(input[2]) must have the same type."
        );
    }

    if (get_size(input_types[1]) == 0)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::infer_output: unknown type for x and y "
            "operands."
        );
    }

    output_types[0] = input_types[1];
}

void select_operation_data_type_policy::validate(
    span<const numerical_type> output_types,
    span<const numerical_type> input_types
) const
{
    if (output_types.size() != 1)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: expected 1 output operand."
        );
    }
    if (input_types.size() != 3)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: expected 3 input operands "
            "(mask, x, y)."
        );
    }

    if (input_types[0] != numerical_type::boolean)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: mask (input[0]) must be "
            "boolean."
        );
    }

    const auto value_type = output_types[0];

    if (get_size(value_type) == 0)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: unknown output type."
        );
    }

    if (input_types[1] != value_type)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: x (input[1]) must match the "
            "output type."
        );
    }

    if (input_types[2] != value_type)
    {
        throw std::invalid_argument(
            "select_operation_data_type_policy::validate: y (input[2]) must match the "
            "output type."
        );
    }
}

const select_operation_data_type_policy& select_operation_data_type_policy::get() noexcept
{
    static const select_operation_data_type_policy instance;
    return instance;
}

} // namespace multidimensional
} // namespace xmipp4
