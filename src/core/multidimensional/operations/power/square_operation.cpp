// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power/square_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string square_operation::get_name() const
{
    return "square";
}

operation_arity square_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
