// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic/abs_operation.hpp>

#include <core/multidimensional/operation_data_type_policies/abs_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string abs_operation::get_name() const
{
    return "abs";
}

operation_arity abs_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

const operation_data_type_policy& 
abs_operation::get_operation_data_type_policy() const noexcept
{
	return abs_operation_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
