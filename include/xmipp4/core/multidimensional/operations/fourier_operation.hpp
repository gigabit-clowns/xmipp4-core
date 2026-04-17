// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/multidimensional/operation.hpp>
#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace multidimensional
{

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
 */
class XMIPP4_CORE_API fourier_operation
	: public operation
{
public:
	// TODO axes
};

} // namespace multidimensional
} // namespace xmipp4
