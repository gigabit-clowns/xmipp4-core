// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/logical/logical_and_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string logical_and_operation::get_name() const
{
    return "logical_and";
}

operation_arity logical_and_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
