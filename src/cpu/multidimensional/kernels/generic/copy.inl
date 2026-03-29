// SPDX-License-Identifier: GPL-3.0-only

#include "copy.hpp"

namespace xmipp4
{
namespace multidimensional
{

template <typename T>
inline
void copy_strided(
	T* result, 
	const T* input, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t input_stride 
)
{
	std::ptrdiff_t result_index = 0;
	std::ptrdiff_t input_index = 0;
	for (std::size_t i = 0; i < count; ++i)
	{
		result[result_index] = input[input_index];

		result_index += result_stride;
		input_index += input_stride;
	}
}

template <typename T, typename Q>
void cast_contiguous(
	T* result, 
	const Q* input, 
	std::size_t count
)
{
	for (std::size_t i = 0; i < count; ++i)
	{
		result[i] = static_cast<T>(input[i]);
	}
}

template <typename T, typename Q>
inline
void cast_strided(
	T* result, 
	const Q* input, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	std::ptrdiff_t input_stride 
)
{
	std::ptrdiff_t result_index = 0;
	std::ptrdiff_t input_index = 0;
	for (std::size_t i = 0; i < count; ++i)
	{
		result[result_index] = static_cast<T>(input[input_index]);

		result_index += result_stride;
		input_index += input_stride;
	}
}

template <typename T>
inline
void fill_strided(
	T* result, 
	std::size_t count, 
	std::ptrdiff_t result_stride, 
	const T& value
)
{
	std::ptrdiff_t result_index = 0;
	for (std::size_t i = 0; i < count; ++i)
	{
		result[result_index] = value;

		result_index += result_stride;
	}
}

} // namespace multidimensional
} // namespace xmipp4
