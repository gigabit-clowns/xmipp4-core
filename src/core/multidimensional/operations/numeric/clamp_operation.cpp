// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/numeric/clamp_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string clamp_operation::get_name() const
{
    return "clamp";
}

operation_arity clamp_operation::get_arity() const noexcept
{
    return operation_arity::ternary();
}

} // namespace multidimensional
} // namespace xmipp4
