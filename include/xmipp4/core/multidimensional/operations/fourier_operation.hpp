// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/span.hpp>

#include <cstddef>
#include <vector>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Direction of a complex-to-complex FFT.
 */
enum class fft_direction
{
	forward, ///< Direct Fourier transform.
	backward ///< Inverse Fourier transform.
};

/**
 * @brief Normalization conventions for Fast Fourier Transforms.
 */
enum class fft_normalization
{
	none, ///< No normalization. ifft(fft(x)) will produce results scaled by N.
	ortho, ///< Forward and backward transfroms divide the result by √N.
	forward, ///< Forward transform divides by N, backward does nothing.
	backward, ///< Backward transform divides by N, forward does nothing.
};

/**
 * @brief Base class for fourier transform operations.
 *
 * Stores the list of axes along which the transform is computed and the
 * normalization convention.
 */
class XMIPP4_CORE_API fourier_operation
	: public operation
{
public:
	/**
	 * @brief Construct the base with a list of transform axes.
	 *
	 * @param axes  Indices of the axes to transform. Must not be empty.
	 * @param normalization Normalization convention applied to the transform.
	 */
	explicit fourier_operation(
		std::vector<std::size_t> axes,
		fft_normalization normalization = fft_normalization::none
	);

	~fourier_operation() override;

	/**
	 * @brief Return the axes along which the transform is computed.
	 */
	span<const std::size_t> get_axes() const noexcept;

	/**
	 * @brief Return the normalization convention.
	 */
	fft_normalization get_normalization() const noexcept;

private:
	std::vector<std::size_t> m_axes;
	fft_normalization m_normalization;
};

/**
 * @brief Return the scale factor for the forward FFT output under @p convention.
 *
 * Multiply the forward-transform output by this value to normalize it:
 * - @c none     → 1
 * - @c ortho    → 1/√n
 * - @c forward  → 1/n
 * - @c backward → 1
 *
 * @param convention Normalization convention.
 * @param n Transform size (product of transformed extents).
 * @return Scale factor.
 */
XMIPP4_CORE_API
double get_forward_fft_normalization_factor(
	fft_normalization convention,
	std::size_t n
);

/**
 * @brief Return the scale factor for the backward (inverse) FFT output
 * under @p convention.
 *
 * Multiply the backward-transform output by this value to normalize it:
 * - @c none     → 1
 * - @c ortho    → 1/√n
 * - @c forward  → 1
 * - @c backward → 1/n
 *
 * @param convention Normalization convention.
 * @param n Transform size (product of transformed extents).
 * @return Scale factor.
 */
XMIPP4_CORE_API
double get_backward_fft_normalization_factor(
	fft_normalization convention,
	std::size_t n
);

/**
 * @brief Return the scale factor for a given FFT direction under @p convention.
 *
 * Dispatches to @ref get_forward_fft_normalization_factor or
 * @ref get_backward_fft_normalization_factor based on @p direction.
 *
 * @param convention Normalization convention.
 * @param direction Transform direction.
 * @param n Transform size (product of transformed extents).
 * @return Scale factor.
 */
XMIPP4_CORE_API
double get_fft_normalization_factor(
	fft_normalization convention,
	fft_direction direction,
	std::size_t n
);

} // namespace multidimensional
} // namespace xmipp4
