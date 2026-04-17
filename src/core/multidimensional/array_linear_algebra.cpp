// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_linear_algebra.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/linear_algebra/matmul_operation.hpp>
#include <xmipp4/core/multidimensional/operations/linear_algebra/matvec_operation.hpp>
#include <xmipp4/core/multidimensional/operations/linear_algebra/dot_operation.hpp>
#include <xmipp4/core/multidimensional/operations/linear_algebra/outer_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

array matmul(
    array_view a,
    array_view b,
    const execution_context &context,
    array *out
)
{
    return execute_binary(
        matmul_operation(),
        std::move(a),
        std::move(b),
        context,
        out
    );
}

array matvec(
    array_view a,
    array_view x,
    const execution_context &context,
    array *out
)
{
    return execute_binary(
        matvec_operation(),
        std::move(a),
        std::move(x),
        context,
        out
    );
}

array dot(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out
)
{
    return execute_binary(
        dot_operation(),
        std::move(x),
        std::move(y),
        context,
        out
    );
}

array outer(
    array_view x,
    array_view y,
    const execution_context &context,
    array *out
)
{
    return execute_binary(
        outer_operation(),
        std::move(x),
        std::move(y),
        context,
        out
    );
}

} // namespace multidimensional
} // namespace xmipp4
