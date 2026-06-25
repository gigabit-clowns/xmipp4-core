// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/min_masked_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string min_masked_operation::get_name() const 
{
    return "min_masked";
}
operation_arity min_masked_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
