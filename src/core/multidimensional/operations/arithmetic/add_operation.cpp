// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/arithmetic/add_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string add_operation::get_name() const
{
    return "add";
}

operation_arity add_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
