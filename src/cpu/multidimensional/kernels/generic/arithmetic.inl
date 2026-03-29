// SPDX-License-Identifier: GPL-3.0-only

#include "arithmetic.hpp"

namespace xmipp4
{
namespace multidimensional
{

template <typename T>
inline
void add_strided(
	T* result, 
	const T* x, 
	const T* y, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t x_stride, 
	std::ptrdiff_t y_stride
)
{
	std::ptrdiff_t result_index = 0;
	std::ptrdiff_t x_index = 0;
	std::ptrdiff_t y_index = 0;
	for (std::size_t i = 0; i < count; ++i)
	{
		result[result_index] = x[x_index] + y[y_index];

		result_index += result_stride;
		x_index += x_stride;
		y_index += y_stride;
	}
}

template <typename T>
inline
void add_constant_strided(
	T* result, 
	const T* input, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t input_stride, 
	const T& value
)
{
	std::ptrdiff_t result_index = 0;
	std::ptrdiff_t input_index = 0;
	for (std::size_t i = 0; i < count; ++i)
	{
		result[result_index] = input[input_index] + value;

		result_index += result_stride;
		input_index += input_stride;
	}
}

} // namespace multidimensional
} // namespace xmipp4
