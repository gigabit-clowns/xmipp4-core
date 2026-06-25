// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/power/log_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string log_operation::get_name() const
{
    return "log";
}

operation_arity log_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
