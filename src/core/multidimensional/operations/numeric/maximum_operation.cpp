// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/numeric/maximum_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string maximum_operation::get_name() const
{
    return "maximum";
}

operation_arity maximum_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
