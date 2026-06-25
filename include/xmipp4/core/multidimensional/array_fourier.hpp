// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "array.hpp"
#include "array_view.hpp"
#include "operations/fourier_operation.hpp"

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

class execution_context;

XMIPP4_CORE_API
array fft(
    array_view x,
	std::vector<std::size_t> axes,
    const execution_context &context,
	fft_normalization norm = fft_normalization::none,
    array *out = nullptr
);

XMIPP4_CORE_API
array rfft(
    array_view x,
	std::vector<std::size_t> axes,
    const execution_context &context,
	fft_normalization norm = fft_normalization::none,
    array *out = nullptr
);

XMIPP4_CORE_API
array ifft(
    array_view x,
	std::vector<std::size_t> axes,
    const execution_context &context,
	fft_normalization norm = fft_normalization::none,
    array *out = nullptr
);

XMIPP4_CORE_API
array irfft(
    array_view x,
	std::vector<std::size_t> axes,
    const execution_context &context,
	bool is_odd = false,
	fft_normalization norm = fft_normalization::none,
    array *out = nullptr
);

} // namespace multidimensional
} // namespace xmipp4
