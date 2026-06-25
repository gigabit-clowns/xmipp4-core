// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/trigonometric/tan_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string tan_operation::get_name() const
{
    return "tan";
}

operation_arity tan_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
