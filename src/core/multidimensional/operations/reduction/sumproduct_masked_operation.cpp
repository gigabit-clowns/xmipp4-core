// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/sumproduct_masked_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string sumproduct_masked_operation::get_name() const
{
    return "sumproduct_masked";
}
operation_arity sumproduct_masked_operation::get_arity() const noexcept
{
    return operation_arity::ternary();
}

} // namespace multidimensional
} // namespace xmipp4
