// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/trigonometric/asin_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string asin_operation::get_name() const
{
    return "asin";
}

operation_arity asin_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
