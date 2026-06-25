// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/numeric/minimum_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string minimum_operation::get_name() const
{
    return "minimum";
}

operation_arity minimum_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
