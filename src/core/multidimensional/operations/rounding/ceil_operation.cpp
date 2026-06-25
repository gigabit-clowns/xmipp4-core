// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/rounding/ceil_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string ceil_operation::get_name() const
{
    return "ceil";
}

operation_arity ceil_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
