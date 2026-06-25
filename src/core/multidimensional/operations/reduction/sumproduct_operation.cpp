// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/sumproduct_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string sumproduct_operation::get_name() const 
{
    return "sumproduct";
}
operation_arity sumproduct_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
