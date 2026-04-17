// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/linear_algebra_operation.hpp>

#include <xmipp4/core/multidimensional/data_type_policies/same_arithmetic_data_type_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

const data_type_policy&
linear_algebra_operation::get_data_type_policy() const noexcept
{
    return same_arithmetic_data_type_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
