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




/**
 * @brief Helper class to pass type information to a function.
 * 
 * Eventually to be replaced with std::type_identity in C++20.
 * 
 * @tparam T Type passed.
 */
template <typename T>
class type_tag
{
public:
	using type = T;

};

/**
 * @brief Dispatch a function with the numerical types.
 * 
 * This function dispatches a set of numerical_type-s and calls the provided
 * visitor with the dispatched types wrapped around a type_tag. In case a type
 * fails conversion to a C++ type, type_tag<void> is passed.
 * 
 * @tparam F Functor to be dispatched.
 * @tparam Types any amount of numeric_type-s
 * @param visitor Function to be called with the dispatched types.
 * @param types Types to be dispatched.
 * @return auto The result of calling visitor(types...)
 * 
 * @see numerical_type
 * @see type_tag
 */
template <typename F, typename... Types>
XMIPP4_CONSTEXPR
auto dispatch_numerical_types(F&& visitor, Types&& ...types);

} // namespace xmipp4
