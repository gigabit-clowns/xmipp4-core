// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/operations/assignment/copy_operation.hpp>

#include <xmipp4/core/operations/shape_policies/elementwise_shape_policy.hpp>

#include <core/operations/data_type_policies/copy_data_type_policy.hpp>

namespace xmipp4
{
namespace operations
{

std::string copy_operation::get_name() const
{
    return "copy";
}

const shape_policy&
copy_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy&
copy_operation::get_operation_data_type_policy() const noexcept
{
	return copy_data_type_policy::get();
}

operation_arity copy_operation::get_arity() const noexcept
{
    return operation_arity(OUTPUT_OPERAND_COUNT, INPUT_OPERAND_COUNT);
}

} // namespace operations
} // namespace xmipp4
