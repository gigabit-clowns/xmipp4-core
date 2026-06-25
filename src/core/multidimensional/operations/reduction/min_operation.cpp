// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/min_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string min_operation::get_name() const 
{
    return "min";
}
operation_arity min_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
