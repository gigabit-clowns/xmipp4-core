// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/trigonometric/atan_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string atan_operation::get_name() const
{
    return "atan";
}

operation_arity atan_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
