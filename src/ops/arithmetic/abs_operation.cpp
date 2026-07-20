// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/ops/arithmetic/abs_operation.hpp>

#include <xmipp4/ops/policies/elementwise_operation_shape_policy.hpp>
#include <xmipp4/ops/policies/homogeneous_operation_data_type_policy.hpp>

#include <ops/policies/abs_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace ops
{

std::string abs_operation::get_name() const
{
	return "abs";
}

const operation_shape_policy&
abs_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy&
abs_operation::get_operation_data_type_policy() const noexcept
{
	return abs_operation_data_type_policy::get();
}

operation_arity abs_operation::get_arity() const noexcept
{
	return operation_arity(OUTPUT_OPERAND_COUNT, INPUT_OPERAND_COUNT);
}

} // namespace ops
} // namespace xmipp4
