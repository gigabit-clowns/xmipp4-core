// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/operations/assignment/fill_operation.hpp>

#include <xmipp4/core/operations/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/operations/data_type_policies/homogeneous_data_type_policy.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>
#include <xmipp4/core/numerical_type.hpp>

namespace xmipp4
{
namespace operations
{

fill_operation::fill_operation(const scalar_value &fill_value) noexcept
	: m_fill_value(fill_value)
{
}

fill_operation::~fill_operation() = default;

std::string fill_operation::get_name() const
{
    return "fill";
}

const shape_policy&
fill_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_shape_policy::get();
}

const data_type_policy&
fill_operation::get_operation_data_type_policy() const noexcept
{
	return homogeneous_data_type_policy::get();
}

operation_arity fill_operation::get_arity() const noexcept
{
    return operation_arity(OUTPUT_OPERAND_COUNT, INPUT_OPERAND_COUNT);
}

const scalar_value& fill_operation::get_fill_value() const noexcept
{
	return m_fill_value;
}

} // namespace operations
} // namespace xmipp4
