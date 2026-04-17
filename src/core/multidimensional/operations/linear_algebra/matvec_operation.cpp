// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/linear_algebra/matvec_operation.hpp>

#include <core/multidimensional/shape_policies/matvec_shape_policy.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string matvec_operation::get_name() const
{
    return "matvec";
}

std::size_t matvec_operation::get_output_count() const noexcept
{
    return 1;
}

std::size_t matvec_operation::get_input_count() const noexcept
{
    return 2;
}

const shape_policy& matvec_operation::get_shape_policy() const noexcept
{
    return matvec_shape_policy::get();
}

} // namespace multidimensional
} // namespace xmipp4
