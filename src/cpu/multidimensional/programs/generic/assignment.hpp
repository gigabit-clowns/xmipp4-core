// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>

namespace xmipp4
{
namespace multidimensional
{

template <typename T, typename ResultStride, typename InputStride>
void copy(
	T* result, 
	const T* input, 
	std::size_t count, 
	ResultStride result_stride, 
	InputStride input_stride
);

template <typename T, typename Q, typename ResultStride, typename InputStride>
void cast(
	T* result, 
	const Q* input, 
	std::size_t count, 
	ResultStride result_stride, 
	InputStride input_stride
);

template <typename T, typename ResultStride>
void fill(
	T* result, 
	std::size_t count, 
	ResultStride result_stride, 
	const T& value
);

} // namespace multidimensional
} // namespace xmipp4

#include "assignment.inl"
