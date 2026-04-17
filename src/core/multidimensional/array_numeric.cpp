// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_numeric.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/numeric/minimum_operation.hpp>
#include <xmipp4/core/multidimensional/operations/numeric/maximum_operation.hpp>
#include <xmipp4/core/multidimensional/operations/numeric/clamp_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

array minimum(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out
)
{
    return execute_binary(
        minimum_operation(),
        std::move(x),
        std::move(y),
        context,
        out
    );
}

array maximum(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out
)
{
    return execute_binary(
        maximum_operation(),
        std::move(x),
        std::move(y),
        context,
        out
    );
}

array clamp(
    array_view x,
    array_view lo,
    array_view hi,
    const execution_context &context,
    array *out
)
{
    return execute_ternary(
        clamp_operation(),
        std::move(x),
        std::move(lo),
        std::move(hi),
        context,
        out
    );
}

} // namespace multidimensional
} // namespace xmipp4
