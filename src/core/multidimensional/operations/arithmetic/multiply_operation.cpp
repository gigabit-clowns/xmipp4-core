// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic/multiply_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string multiply_operation::get_name() const
{
    return "multiply";
}

operation_arity multiply_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
