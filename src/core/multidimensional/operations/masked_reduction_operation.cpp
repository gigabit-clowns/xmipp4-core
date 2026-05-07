// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/masked_reduction_operation.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/masked_operation_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const operation_data_type_policy&
masked_reduction_operation::get_operation_data_type_policy() const noexcept
{
    return masked_operation_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
