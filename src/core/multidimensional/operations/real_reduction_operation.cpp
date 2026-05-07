// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/real_reduction_operation.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_real_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const operation_data_type_policy&
real_reduction_operation::get_operation_data_type_policy() const noexcept
{
    return homogeneous_real_operation_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
