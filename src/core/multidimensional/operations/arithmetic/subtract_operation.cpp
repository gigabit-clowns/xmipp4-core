// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic/subtract_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string subtract_operation::get_name() const
{
    return "subtract";
}

operation_arity subtract_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
