// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/multidimensional/array_fourier.hpp>

#include <xmipp4/core/multidimensional/operation_execute.hpp>
#include <xmipp4/core/multidimensional/operations/fourier/fft_c2c_operation.hpp>
#include <xmipp4/core/multidimensional/operations/fourier/fft_r2c_operation.hpp>
#include <xmipp4/core/multidimensional/operations/fourier/fft_c2r_operation.hpp>

namespace xmipp4
{
namespace multidimensional
{

array fft(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    fft_normalization norm,
    array *out
)
{
    return execute_unary(
        fft_c2c_operation(
            std::vector<std::size_t>(axes.begin(), axes.end()),
            fft_direction::forward,
            norm
        ),
        x, context, out
    );
}

array rfft(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    fft_normalization norm,
    array *out
)
{
    return execute_unary(
        fft_r2c_operation(
            std::vector<std::size_t>(axes.begin(), axes.end()),
            norm
        ),
        x, context, out
    );
}

array ifft(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    fft_normalization norm,
    array *out
)
{
    return execute_unary(
        fft_c2c_operation(
            std::vector<std::size_t>(axes.begin(), axes.end()),
            fft_direction::backward,
            norm
        ),
        x, context, out
    );
}

array irfft(
    array_view x,
    span<const std::size_t> axes,
    const execution_context &context,
    bool is_odd,
    fft_normalization norm,
    array *out
)
{
    return execute_unary(
        fft_c2r_operation(
            std::vector<std::size_t>(axes.begin(), axes.end()),
            is_odd,
            norm
        ),
        x, context, out
    );
}

} // namespace multidimensional
} // namespace xmipp4
