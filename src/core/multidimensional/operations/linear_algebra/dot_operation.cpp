// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/linear_algebra/dot_operation.hpp>

#include <core/multidimensional/shape_policies/dot_shape_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string dot_operation::get_name() const
{
    return "dot";
}

std::size_t dot_operation::get_output_count() const noexcept
{
    return 1;
}

std::size_t dot_operation::get_input_count() const noexcept
{
    return 2;
}

const shape_policy& dot_operation::get_shape_policy() const noexcept
{
    return dot_shape_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
