// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/operations/reduction/rms_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

std::string rms_operation::get_name() const 
{
    return "rms";
}
operation_arity rms_operation::get_arity() const noexcept
{
    return operation_arity::unary();
}

} // namespace multidimensional
} // namespace xmipp4
