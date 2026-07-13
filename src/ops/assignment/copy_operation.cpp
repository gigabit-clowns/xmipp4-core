// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/assignment/copy_operation.hpp>

#include <xmipp4/ops/policies/elementwise_operation_shape_policy.hpp>

#include <ops/policies/copy_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace ops
{

std::string copy_operation::get_name() const
{
    return "copy";
}

const operation_shape_policy&
copy_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy&
copy_operation::get_operation_data_type_policy() const noexcept
{
	return copy_operation_data_type_policy::get();
}

operation_arity copy_operation::get_arity() const noexcept
{
    return operation_arity(OUTPUT_OPERAND_COUNT, INPUT_OPERAND_COUNT);
}

} // namespace ops
} // namespace xmipp4
