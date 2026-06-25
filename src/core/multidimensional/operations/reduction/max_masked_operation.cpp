// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/max_masked_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string max_masked_operation::get_name() const 
{
    return "max_masked";
}
operation_arity max_masked_operation::get_arity() const noexcept
{
    return operation_arity::binary();
}

} // namespace multidimensional
} // namespace xmipp4
