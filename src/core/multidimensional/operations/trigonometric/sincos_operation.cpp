// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/trigonometric/sincos_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string sincos_operation::get_name() const
{
    return "sincos";
}

operation_arity sincos_operation::get_arity() const noexcept
{
    return operation_arity(2, 1);
}

} // namespace multidimensional
} // namespace xmipp4
