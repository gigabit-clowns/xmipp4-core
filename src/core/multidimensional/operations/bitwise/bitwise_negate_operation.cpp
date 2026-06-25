// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_negate_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string bitwise_negate_operation::get_name() const
{
    return "bitwise_negate";
}

operation_arity bitwise_negate_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
