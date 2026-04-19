// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/linear_algebra_operation.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_arithmetic_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const operation_data_type_policy&
linear_algebra_operation::get_operation_data_type_policy() const noexcept
{
    return homogeneous_arithmetic_operation_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
