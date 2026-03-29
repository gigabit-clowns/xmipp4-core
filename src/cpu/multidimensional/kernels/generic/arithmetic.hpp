// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>

namespace xmipp4
{
namespace multidimensional
{

template <typename T>
void add_strided(
	T* result, 
	const T* x, 
	const T* y, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t x_stride, 
	std::ptrdiff_t y_stride
);

template <typename T>
void add_inplace_strided(
	T* result, 
	const T* x, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t x_stride
);

template <typename T>
void add_constant_strided(
	T* result, 
	const T* input, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t input_stride, 
	const T& value
);

} // namespace multidimensional
} // namespace xmipp4

#include "arithmetic.inl"
