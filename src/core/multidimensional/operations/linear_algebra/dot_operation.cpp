// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/linear_algebra/dot_operation.hpp>

#include <core/multidimensional/operation_shape_policies/dot_operation_shape_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string dot_operation::get_name() const
{
    return "dot";
}

operation_arity dot_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

const operation_shape_policy& 
dot_operation::get_operation_shape_policy() const noexcept
{
    return dot_operation_shape_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
