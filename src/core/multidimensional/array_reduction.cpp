// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_reduction.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/sum_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/sum_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/max_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/max_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/min_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/min_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/mean_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/mean_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/variance_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/variance_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/stddev_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/stddev_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/sum_square_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/sum_square_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/mean_square_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/mean_square_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/rms_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/rms_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/norm_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/norm_masked_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/sumproduct_operation.hpp>
#include <xmipp4/core/multidimensional/operations/reduction/sumproduct_masked_operation.hpp>

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

static std::vector<std::size_t> to_vector(span<const std::size_t> axes)
{
    return std::vector<std::size_t>(axes.begin(), axes.end());
}

array sum(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            sum_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        sum_operation(to_vector(axes), keep_dim),
        x,
        context,
        out
    );
}

array max(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            max_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        max_operation(to_vector(axes), keep_dim),
        x,
        context,
        out
    );
}

array min(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            min_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        min_operation(to_vector(axes), keep_dim),
        x,
        context,
        out
    );
}

array mean(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            mean_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        mean_operation(to_vector(axes), keep_dim),
        x,
        context,
        out
    );
}

array variance(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    std::size_t ddof,
    const array_view * /*mean*/,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            variance_masked_operation(to_vector(axes), ddof, keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        variance_operation(to_vector(axes), ddof, keep_dim),
        x,
        context,
        out
    );
}

array stddev(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    std::size_t ddof,
    const array_view * /*mean*/,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            stddev_masked_operation(to_vector(axes), ddof, keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        stddev_operation(to_vector(axes), ddof, keep_dim),
        x,
        context,
        out
    );
}

array sum_square(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            sum_square_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        sum_square_operation(to_vector(axes), keep_dim),
        x,
        context,
        out
    );
}

array mean_square(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            mean_square_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        mean_square_operation(to_vector(axes), keep_dim),
        x,
        context,
        out
    );
}

array rms(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            rms_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        rms_operation(to_vector(axes), keep_dim),
        x,
        context,
        out
    );
}

array norm(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_binary(
            norm_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            context,
            out
        );
    }
    return execute_unary(
        norm_operation(to_vector(axes), keep_dim),
        x,
        context,
        out
    );
}

array sumproduct(
    array_view x,
    array_view y,
    span<const std::size_t> axes,
    const execution_context &context,
    bool keep_dim,
    const array_view *where,
    array *out
)
{
    if (where)
    {
        return execute_ternary(
            sumproduct_masked_operation(to_vector(axes), keep_dim),
            where->share(),
            std::move(x),
            std::move(y),
            context,
            out
        );
    }
    return execute_binary(
        sumproduct_operation(to_vector(axes), keep_dim),
        std::move(x),
        std::move(y),
        context,
        out
    );
}

} // namespace multidimensional
} // namespace xmipp4
