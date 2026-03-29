// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>

namespace xmipp4
{
namespace multidimensional
{

template <typename T>
T sum_strided(
	const T* x, 
	std::size_t count, 
	std::ptrdiff_t x_stride
);

} // namespace multidimensional
} // namespace xmipp4

#include "arithmetic.inl"
