// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "numerical_type.hpp"
#include "fixed_float.hpp"

#include <complex>

namespace xmipp4
{

/**
 * @brief Convert static types to numerical_type.
 * 
 * The numerical_type of the provided static type is stored as 
 * numerical_type_of<T>::value
 * 
 * @tparam T The static type to be converted.
 */
template <typename T>
struct numerical_type_of;

template <>
struct numerical_type_of<bool>
	: std::integral_constant<numerical_type, numerical_type::boolean>
{
};

template <>
struct numerical_type_of<char>
	: std::integral_constant<numerical_type, numerical_type::char8>
{
};

template <>
struct numerical_type_of<std::int8_t>
	: std::integral_constant<numerical_type, numerical_type::int8>
{
};

template <>
struct numerical_type_of<std::uint8_t>
	: std::integral_constant<numerical_type, numerical_type::uint8>
{
};

template <>
struct numerical_type_of<std::int16_t>
	: std::integral_constant<numerical_type, numerical_type::int16>
{
};

template <>
struct numerical_type_of<std::uint16_t>
	: std::integral_constant<numerical_type, numerical_type::uint16>
{
};

template <>
struct numerical_type_of<std::int32_t>
	: std::integral_constant<numerical_type, numerical_type::int32>
{
};

template <>
struct numerical_type_of<std::uint32_t>
	: std::integral_constant<numerical_type, numerical_type::uint32>
{
};

template <>
struct numerical_type_of<std::int64_t>
	: std::integral_constant<numerical_type, numerical_type::int64>
{
};

template <>
struct numerical_type_of<std::uint64_t>
	: std::integral_constant<numerical_type, numerical_type::uint64>
{
};

template <>
struct numerical_type_of<float16_t>
	: std::integral_constant<numerical_type, numerical_type::float16>
{
};

template <>
struct numerical_type_of<float32_t>
	: std::integral_constant<numerical_type, numerical_type::float32>
{
};

template <>
struct numerical_type_of<float64_t>
	: std::integral_constant<numerical_type, numerical_type::float64>
{
};

template <>
struct numerical_type_of<std::complex<float16_t>>
	: std::integral_constant<numerical_type, numerical_type::complex_float16>
{
};

template <>
struct numerical_type_of<std::complex<float32_t>>
	: std::integral_constant<numerical_type, numerical_type::complex_float32>
{
};

template <>
struct numerical_type_of<std::complex<float64_t>>
	: std::integral_constant<numerical_type, numerical_type::complex_float64>
{
};

} // namespace xmipp4
