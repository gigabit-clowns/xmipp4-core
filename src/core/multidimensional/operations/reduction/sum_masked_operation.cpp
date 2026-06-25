// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/sum_masked_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string sum_masked_operation::get_name() const 
{
    return "sum_masked";
}
operation_arity sum_masked_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
