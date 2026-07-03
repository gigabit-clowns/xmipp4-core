// SPDX-License-Identifier: GPL-3.0-only

#include "assignment.hpp"

#include "helpers/strided_pointer_iterator.hpp"

#include <algorithm>

namespace xmipp4
{
namespace multidimensional
{

template <typename T, typename ResultStride, typename InputStride>
inline
void copy(
	T* result, 
	const T* input, 
	std::size_t count, 
	ResultStride result_stride, 
	InputStride input_stride
)
{
	std::copy_n(
		make_strided_pointer_iterator(input, input_stride), 
		count, 
		make_strided_pointer_iterator(result, result_stride)
	);
}

template <typename T, typename Q, typename ResultStride, typename InputStride>
inline
void cast(
	T* result, 
	const Q* input, 
	std::size_t count, 
	ResultStride result_stride, 
	InputStride input_stride
)
{
	auto result_ite = make_strided_pointer_iterator(result, result_stride);
	auto input_ite = make_strided_pointer_iterator(input, input_stride);

	for (std::size_t i = 0; i < count; ++i)
	{
		*result_ite = *input_ite;
		++result_ite;
		++input_ite;
	}
}

template <typename T, typename ResultStride>
inline
void fill(
	T* result, 
	std::size_t count, 
	ResultStride result_stride, 
	const T& value
)
{
	std::fill_n(
		make_strided_pointer_iterator(result, result_stride),
		count, 
		value
	);
}

} // namespace multidimensional
} // namespace xmipp4
