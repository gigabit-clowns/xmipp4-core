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
std::size_t sumproduct_masked_operation::get_output_count() const noexcept
{
    return 1;
}
std::size_t sumproduct_masked_operation::get_input_count() const noexcept
{
    return 3;
}

} // namespace multidimensional
} // namespace xmipp4
