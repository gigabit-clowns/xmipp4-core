// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical/logical_negate_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string logical_negate_operation::get_name() const
{
    return "logical_negate";
}

operation_arity logical_negate_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
