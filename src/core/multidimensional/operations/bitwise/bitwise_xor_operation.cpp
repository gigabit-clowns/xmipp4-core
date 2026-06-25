// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/bitwise/bitwise_xor_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string bitwise_xor_operation::get_name() const
{
    return "bitwise_xor";
}

operation_arity bitwise_xor_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
