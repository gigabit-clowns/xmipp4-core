// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/fixed_float.hpp>

#include <complex>

#include <hwy/highway.h>

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
struct to_hwy_data_type
{
	using type = T;
};

template <>
struct to_hwy_data_type<xmipp4::float16_t>
{
	using type = hwy::float16_t;
};

template <>
struct to_hwy_data_type<std::complex<xmipp4::float16_t>>
{
	using type = std::complex<hwy::float16_t>;
};

template<typename T>
typename to_hwy_data_type<T>::type* to_hwy(T* ptr) noexcept;

template<typename T>
const typename to_hwy_data_type<T>::type* to_hwy(const T* ptr) noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "convert_data_type.inl"
