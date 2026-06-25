// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical/logical_or_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string logical_or_operation::get_name() const
{
    return "logical_or";
}

operation_arity logical_or_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
