// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/linear_algebra/outer_operation.hpp>

#include <core/multidimensional/operation_shape_policies/outer_operation_shape_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string outer_operation::get_name() const
{
    return "outer";
}

std::size_t outer_operation::get_output_count() const noexcept
{
    return 1;
}

std::size_t outer_operation::get_input_count() const noexcept
{
    return 2;
}

const operation_shape_policy& 
outer_operation::get_operation_shape_policy() const noexcept
{
    return outer_operation_shape_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
