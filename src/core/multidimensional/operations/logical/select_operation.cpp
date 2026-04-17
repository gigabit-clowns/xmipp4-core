// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical/select_operation.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include "../../data_type_policies/select_data_type_policy.hpp"

namespace xmipp4
{
namespace multidimensional
{

std::string select_operation::get_name() const
{
    return "select";
}

const shape_policy& select_operation::get_shape_policy() const noexcept
{
    return elementwise_shape_policy::get();
}

const data_type_policy& select_operation::get_data_type_policy() const noexcept
{
    return select_data_type_policy::get();
}

std::size_t select_operation::get_output_count() const noexcept { return 1; }
std::size_t select_operation::get_input_count() const noexcept { return 3; }

} // namespace multidimensional
} // namespace xmipp4
