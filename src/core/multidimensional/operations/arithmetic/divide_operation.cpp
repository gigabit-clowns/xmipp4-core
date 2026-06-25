// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic/divide_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string divide_operation::get_name() const
{
    return "divide";
}

operation_arity divide_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
