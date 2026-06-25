// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power/power_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string power_operation::get_name() const
{
    return "power";
}

operation_arity power_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
