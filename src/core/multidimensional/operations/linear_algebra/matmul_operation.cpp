// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/linear_algebra/matmul_operation.hpp>

#include <core/multidimensional/operation_shape_policies/matmul_operation_shape_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string matmul_operation::get_name() const
{
    return "matmul";
}

operation_arity matmul_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

const operation_shape_policy& 
matmul_operation::get_operation_shape_policy() const noexcept
{
    return matmul_operation_shape_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
