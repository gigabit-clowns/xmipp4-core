// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "array_view.hpp"
#include "operations/fourier_operation.hpp"

#include <xmipp4/core/span.hpp>

namespace xmipp4
{

class execution_context;

namespace multidimensional
{

/**
 * @brief Compute the Fast Fourier Transform (FFT) of an array.
 *
 * @param a Input array. Must be complex-valued.
 * @param axes Axes for which the fourier transform is computed.
 * @param context The device context to handle the allocation.
 * @param norm Normalization convention used with the transform.
 * @param out Optional array to reuse. If provided, must have the same shape and 
 * data type and shape as `x`.
 * @return array The Fourier Transform of the input array. Same shape and
 * data type as the input.
 */
XMIPP4_CORE_API
array fft(
    array_view x,
	span<const std::size_t> axes,
    const execution_context &context,
	fft_normalization norm = fft_normalization::none,
    array *out = nullptr
);

/**
 * @brief Compute the Fast Fourier Transform (FFT) of a real-valued array.
 *
 * @param a Input array. Must be real-valued.
 * @param axes Axes for which the fourier transform is computed.
 * @param context The device context to handle the allocation.
 * @param norm Normalization convention used with the transform.
 * @param out Optional array to reuse. If provided, must have the correct shape
 * and data type.
 * @return array The Fourier Transform of the input array. The shape of the
 * resulting array is the same as the input type except for the last transform
 * dimension, which has a extent of `N//2 + 1`. The data type is the complex
 * equivalent to the input data type.
 */
XMIPP4_CORE_API
array rfft(
    array_view x,
	span<const std::size_t> axes,
    const execution_context &context,
	fft_normalization norm = fft_normalization::none,
    array *out = nullptr
);

/**
 * @brief Compute the inverse Fast Fourier Transform (IFFT) of an array.
 *
 * @param a Input array. Must be complex-valued.
 * @param axes Axes for which the fourier transform is computed.
 * @param context The device context to handle the allocation.
 * @param norm Normalization convention used with the transform.
 * @param out Optional array to reuse. If provided, must have the same shape and 
 * data type and shape as `x`.
 * @return array The inverse Fourier Transform of the input array. Same shape 
 * and data type as the input.
 */
XMIPP4_CORE_API
array ifft(
    array_view x,
	span<const std::size_t> axes,
    const execution_context &context,
	fft_normalization norm = fft_normalization::none,
    array *out = nullptr
);

/**
 * @brief Compute the inverse Fast Fourier Transform (IFFT) of a real-valued 
 * array.
 *
 * @param a Input array. Must be complex-valued.
 * @param axes Axes for which the fourier transform is computed.
 * @param context The device context to handle the allocation.
 * @param norm Normalization convention used with the transform.
 * @param out Optional array to reuse. If provided, must have the correct shape
 * and data type.
 * @return array The Fourier Transform of the input array. The shape of the
 * resulting array is the same as the input type except for the last transform
 * dimension, which has a extent of `(N-1)*2`. The data type is the real 
 * equivalent to the input data type. TODO: Define behavior for odd-sized
 * output arrays.
 */
XMIPP4_CORE_API
array irfft(
    array_view x,
	span<const std::size_t> axes,
    const execution_context &context,
	fft_normalization norm = fft_normalization::none,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
