// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/rounding/trunc_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string trunc_operation::get_name() const
{
    return "trunc";
}

operation_arity trunc_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
