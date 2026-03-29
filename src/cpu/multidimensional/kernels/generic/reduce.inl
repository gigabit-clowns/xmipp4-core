// SPDX-License-Identifier: GPL-3.0-only

#include "arithmetic.hpp"

namespace xmipp4
{
namespace multidimensional
{

template <typename T>
inline
T sum_strided(
	const T* x, 
	std::size_t count, 
	std::ptrdiff_t x_stride
)
{
	T total = {};
	std::ptrdiff_t x_index = 0;
	for (std::size_t i = 0; i < count; ++i)
	{
		total += x[x_index];

		x_index += x_inner_stride;
	}

	return total;
}

} // namespace multidimensional
} // namespace xmipp4
