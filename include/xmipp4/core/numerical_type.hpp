// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/constexpr.hpp"

#include <cstddef>
#include <string_view>
#include <ostream>

namespace xmipp4
{
namespace compute
{

/**
 * @brief Representation of all considered numerical types for
 * computations.
 * 
 */
enum class numerical_type
{
    unknown = -1,

    int8,
    uint8,
    int16,
    uint16,
    int32,
    uint32,
    int64,
    uint64,

    float16,
    brain_float16,
    float32,
    float64,

    complex_float16,
    complex_float32,
    complex_float64,
};

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



XMIPP4_CONSTEXPR const char* to_string(numerical_type type) noexcept;
bool from_string(std::string_view str, numerical_type& type) noexcept;

template<typename T>
std::basic_ostream<T>& 
operator<<(std::basic_ostream<T>& os, numerical_type type);

} // namespace compute
} // namespace xmipp4

#include "numerical_type.inl"
