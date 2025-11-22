// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <half.hpp>

#include <limits>
#include <type_traits>

namespace xmipp4
{

/**
 * @brief Alias for half floating point type.
 * 
 */
using half = half_float::half;

/**
 * @brief 16 bit floating point number representation.
 * 
 */
using float16_t = half;
static_assert(sizeof(float16_t) == 2, "float16_t should be 2 bytes long");
static_assert(std::numeric_limits<float16_t>::is_iec559, "float16_t should be IEC 559 compliant");

/**
 * @brief 32 bit floating point number representation.
 * 
 */
using float32_t = float;
static_assert(sizeof(float32_t) == 4, "float32_t should be 4 bytes long");
static_assert(std::numeric_limits<float32_t>::is_iec559, "float32_t should be IEC 559 compliant");

/**
 * @brief 64 bit floating point number representation.
 * 
 */
using float64_t = double;
static_assert(sizeof(float64_t) == 8, "float64_t should be 8 bytes long");
static_assert(std::numeric_limits<float64_t>::is_iec559, "float64_t should be IEC 559 compliant");

/**
 * @brief Type trait to check if the type is one of the fixed-point 
 * floating point types.
 * 
 * This trait evaluates to true if the type is either float16_t, float32_t, or 
 * float64_t.
 * 
 * @tparam T Type to be checked.
 */
template<typename T>
using is_fixed_float = 
	std::integral_constant<bool, std::is_same<T, float16_t>::value || 
	std::is_same<T, float32_t>::value || 
	std::is_same<T, float64_t>::value >;

} // namespace xmipp4
