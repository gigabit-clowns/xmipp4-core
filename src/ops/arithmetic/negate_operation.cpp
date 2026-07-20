// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/arithmetic/negate_operation.hpp>

#include <xmipp4/ops/policies/elementwise_operation_shape_policy.hpp>
#include <xmipp4/ops/policies/homogeneous_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace ops
{

std::string negate_operation::get_name() const
{
	return "negate";
}

const operation_shape_policy&
negate_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy&
negate_operation::get_operation_data_type_policy() const noexcept
{
	return homogeneous_operation_data_type_policy::get();
}

operation_arity negate_operation::get_arity() const noexcept
{
	return operation_arity(OUTPUT_OPERAND_COUNT, INPUT_OPERAND_COUNT);
}

} // namespace ops
} // namespace xmipp4
