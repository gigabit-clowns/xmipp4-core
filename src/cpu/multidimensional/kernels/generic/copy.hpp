// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>

namespace xmipp4
{
namespace multidimensional
{

template <typename T>
void copy_strided(
	T* result, 
	const T* input, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t input_stride
);

template <typename T, typename Q>
void cast_contiguous(
	T* result, 
	const Q* input, 
	std::size_t count
);

template <typename T, typename Q>
void cast_strided(
	T* result, 
	const Q* input, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t input_stride
);

template <typename T>
void fill_strided(
	T* result, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	const T& value
);

} // namespace multidimensional
} // namespace xmipp4

#include "copy.inl"
