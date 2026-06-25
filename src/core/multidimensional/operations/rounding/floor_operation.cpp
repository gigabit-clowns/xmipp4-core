// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/rounding/floor_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string floor_operation::get_name() const
{
    return "floor";
}

operation_arity floor_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
