// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_rounding.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/rounding/floor_operation.hpp>
#include <xmipp4/core/multidimensional/operations/rounding/ceil_operation.hpp>
#include <xmipp4/core/multidimensional/operations/rounding/round_operation.hpp>
#include <xmipp4/core/multidimensional/operations/rounding/trunc_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

array floor(array_view x, const execution_context &context, array *out)
{
    return execute_unary(floor_operation(), std::move(x), context, out);
}

array ceil(array_view x, const execution_context &context, array *out)
{
    return execute_unary(ceil_operation(), std::move(x), context, out);
}

array round(array_view x, const execution_context &context, array *out)
{
    return execute_unary(round_operation(), std::move(x), context, out);
}

array trunc(array_view x, const execution_context &context, array *out)
{
    return execute_unary(trunc_operation(), std::move(x), context, out);
}

} // namespace multidimensional
} // namespace xmipp4
