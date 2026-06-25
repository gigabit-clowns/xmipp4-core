// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/sum_square_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string sum_square_operation::get_name() const 
{
    return "sum_square";
}
operation_arity sum_square_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
