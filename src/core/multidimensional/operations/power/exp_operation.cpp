// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power/exp_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string exp_operation::get_name() const
{
    return "exp";
}

operation_arity exp_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
