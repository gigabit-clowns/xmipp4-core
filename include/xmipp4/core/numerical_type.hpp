// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "fixed_float.hpp"
#include "platform/constexpr.hpp"

#include <cstddef>
#include <ostream>

namespace xmipp4
{

/**
 * @brief Representation of all considered numerical types for
 * computations.
 * 
 */
enum class numerical_type
{
	unknown = -1,

	char8,

	int8,
	uint8,
	int16,
	uint16,
	int32,
	uint32,
	int64,
	uint64,

	float16,
	float32,
	float64,

	complex_float16,
	complex_float32,
	complex_float64,

	// Add here

	count
};

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
auto visit(F&& visitor, Types&& ...types);

/**
 * @brief Get the size of the numerical type.
 * 
 * Equivalent of calling sizeof with the represented type.
 * 
 * @param type The numerical type.
 * @return std::size_t The size.
 */
XMIPP4_CONSTEXPR std::size_t get_size(numerical_type type) noexcept;

/**
 * @brief Check if the numerical type is unsigned.
 * 
 * A numerical type is unsigned for all uint* types.
 * 
 * @param type The numerical type
 * @return true if type is unsigned.
 * @return false if the type is not unsigned.
 */
XMIPP4_CONSTEXPR bool is_unsigned(numerical_type type) noexcept;

/**
 * @brief Check if the numerical type is integral.
 * 
 * A numerical type is unsigned for all (u)int* types.
 * 
 * @param type The numerical type
 * @return true if type is integral.
 * @return false if the type is not integral.
 */
XMIPP4_CONSTEXPR bool is_integer(numerical_type type) noexcept;

/**
 * @brief Check if the numerical type is floating point.
 * 
 * A numerical type is floating point for all float* types.
 * 
 * @param type The numerical type
 * @return true if type is floating point.
 * @return false if the type is not floating point.
 */
XMIPP4_CONSTEXPR bool is_float(numerical_type type) noexcept;

/**
 * @brief Check if the numerical type is complex type.
 * 
 * @param type The numerical type
 * @return true if type is complex.
 * @return false if the type is not complex.
 */
XMIPP4_CONSTEXPR bool is_complex(numerical_type type) noexcept;

/**
 * @brief Create a complex version, if exists.
 * 
 * Create the complex version of a numerical type, if this exists.
 * Otherwise unknown is returned. This includes an already complex type.
 * 
 * @param type 
 * @return numerical_type.
 */
XMIPP4_CONSTEXPR numerical_type make_complex(numerical_type type) noexcept;

/**
 * @brief Get the common type of two numerical types.
 * 
 * @param type1 First numerical type.
 * @param type2 Second numerical type.
 * @return numerical_type The common type.
 */
XMIPP4_CONSTEXPR 
numerical_type common_type(numerical_type type1, numerical_type type2) noexcept;

XMIPP4_CONSTEXPR 
const char* to_string(numerical_type type) noexcept;

template<typename T>
std::basic_ostream<T>& 
operator<<(std::basic_ostream<T>& os, numerical_type type);

} // namespace xmipp4

#include "numerical_type.inl"
