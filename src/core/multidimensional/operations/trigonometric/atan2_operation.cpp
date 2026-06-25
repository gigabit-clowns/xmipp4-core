// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/trigonometric/atan2_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string atan2_operation::get_name() const
{
    return "atan2";
}

operation_arity atan2_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
