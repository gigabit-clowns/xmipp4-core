// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic/negate_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string negate_operation::get_name() const
{
    return "negate";
}

operation_arity negate_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
