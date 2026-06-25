// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/assignment/fill_operation.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>
#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_operation_data_type_policy.hpp>
#include <xmipp4/core/numerical_type_dispatch.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <fmt/format.h>
#include <fmt/ranges.h>

namespace xmipp4
{
namespace multidimensional
{

fill_operation::fill_operation(scalar_ref fill_value) noexcept
	: m_fill_value(fill_value)
{
}

fill_operation::~fill_operation() = default;

std::string fill_operation::get_name() const
{
    return "fill";
}

const operation_shape_policy& 
fill_operation::get_operation_shape_policy() const noexcept
{
	return elementwise_operation_shape_policy::get();
}

const operation_data_type_policy& 
fill_operation::get_operation_data_type_policy() const noexcept
{
	return homogeneous_operation_data_type_policy::get();
}

operation_arity fill_operation::get_arity() const noexcept
{
    return operation_arity::nullary();
}

scalar_ref fill_operation::get_fill_value() const noexcept
{
	return m_fill_value;
}

} // namespace multidimensional
} // namespace xmipp4
