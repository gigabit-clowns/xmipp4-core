// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/mean_square_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string mean_square_operation::get_name() const 
{
    return "mean_square";
}
operation_arity mean_square_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
