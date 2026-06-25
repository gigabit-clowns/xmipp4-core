// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_and_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string bitwise_and_operation::get_name() const
{
    return "bitwise_and";
}

operation_arity bitwise_and_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
