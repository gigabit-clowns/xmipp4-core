// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/sum_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string sum_operation::get_name() const 
{
    return "sum";
}
operation_arity sum_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
