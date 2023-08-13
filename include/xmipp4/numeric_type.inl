#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file numeric_type.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of numeric_type.hpp
 * @date 2023-08-12
 * 
 */

#include "numeric_type.hpp"

#include <type_traits>
#include <unordered_map>

namespace xmipp4
{

static_assert(sizeof(float16_t) == 2, "float16_t must be 2 bytes long");
static_assert(sizeof(float32_t) == 4, "float32_t must be 4 bytes long");
static_assert(sizeof(float64_t) == 8, "float64_t must be 8 bytes long");

static_assert(sizeof(complex_float16_t) == 4, "complex_float16_t must be 4 bytes long");
static_assert(sizeof(complex_float32_t) == 8, "complex_float32_t must be 8 bytes long");
static_assert(sizeof(complex_float64_t) == 16, "complex_float64_t must be 16 bytes long");

namespace detail
{

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, int8_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::int8;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, int16_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::int16;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, int32_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::int32;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, int64_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::int64;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, uint8_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::uint8;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, uint16_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::uint16;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, uint32_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::uint32;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, uint64_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::uint64;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, float16_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::float16;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, float32_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::float32;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, float64_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::float64;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, bfloat16_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::bfloat16;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, bfloat19_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::bfloat19;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, complex_float16_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::complex_float16;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, complex_float32_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::complex_float32;
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, complex_float64_t>::value, numeric_type>::type
type_of() noexcept
{
    return numeric_type::complex_float64;
}

}

template <typename T>
XMIPP4_INLINE_CONSTEXPR numeric_type type_of() noexcept
{
    return detail::type_of<T>();
}

template <typename T>
XMIPP4_INLINE_CONSTEXPR numeric_type type_of(const T&) noexcept
{
    return type_of<T>();
}



XMIPP4_INLINE_CONSTEXPR std::size_t get_sizeof(numeric_type type) noexcept
{
    switch (type)
    {
    case numeric_type::int8: return sizeof(int8_t);
    case numeric_type::int16: return sizeof(int16_t);
    case numeric_type::int32: return sizeof(int32_t);
    case numeric_type::int64: return sizeof(int64_t);
    case numeric_type::uint8: return sizeof(uint8_t);
    case numeric_type::uint16: return sizeof(uint16_t);
    case numeric_type::uint32: return sizeof(uint32_t);
    case numeric_type::uint64: return sizeof(uint64_t);
    case numeric_type::float16: return sizeof(float16_t);
    case numeric_type::float32: return sizeof(float32_t);
    case numeric_type::float64: return sizeof(float64_t);
    case numeric_type::bfloat16: return sizeof(bfloat16_t);
    case numeric_type::bfloat19: return sizeof(bfloat19_t);
    case numeric_type::complex_float16: return sizeof(complex_float16_t);
    case numeric_type::complex_float32: return sizeof(complex_float32_t);
    case numeric_type::complex_float64: return sizeof(complex_float64_t);
    }
}

XMIPP4_INLINE_CONSTEXPR std::size_t get_alignof(numeric_type type) noexcept
{
    switch (type)
    {
    case numeric_type::int8: return alignof(int8_t);
    case numeric_type::int16: return alignof(int16_t);
    case numeric_type::int32: return alignof(int32_t);
    case numeric_type::int64: return alignof(int64_t);
    case numeric_type::uint8: return alignof(uint8_t);
    case numeric_type::uint16: return alignof(uint16_t);
    case numeric_type::uint32: return alignof(uint32_t);
    case numeric_type::uint64: return alignof(uint64_t);
    case numeric_type::float16: return alignof(float16_t);
    case numeric_type::float32: return alignof(float32_t);
    case numeric_type::float64: return alignof(float64_t);
    case numeric_type::bfloat16: return alignof(bfloat16_t);
    case numeric_type::bfloat19: return alignof(bfloat19_t);
    case numeric_type::complex_float16: return alignof(complex_float16_t);
    case numeric_type::complex_float32: return alignof(complex_float32_t);
    case numeric_type::complex_float64: return alignof(complex_float64_t);
    }
}

XMIPP4_INLINE_CONSTEXPR const std::type_info& 
get_typeid(numeric_type type) noexcept
{
    switch (type)
    {
    case numeric_type::int8: return typeid(int8_t);
    case numeric_type::int16: return typeid(int16_t);
    case numeric_type::int32: return typeid(int32_t);
    case numeric_type::int64: return typeid(int64_t);
    case numeric_type::uint8: return typeid(uint8_t);
    case numeric_type::uint16: return typeid(uint16_t);
    case numeric_type::uint32: return typeid(uint32_t);
    case numeric_type::uint64: return typeid(uint64_t);
    case numeric_type::float16: return typeid(float16_t);
    case numeric_type::float32: return typeid(float32_t);
    case numeric_type::float64: return typeid(float64_t);
    case numeric_type::bfloat16: return typeid(bfloat16_t);
    case numeric_type::bfloat19: return typeid(bfloat19_t);
    case numeric_type::complex_float16: return typeid(complex_float16_t);
    case numeric_type::complex_float32: return typeid(complex_float32_t);
    case numeric_type::complex_float64: return typeid(complex_float64_t);
    }
}

XMIPP4_INLINE_CONSTEXPR bool is_int(numeric_type type) noexcept
{
    switch (type)
    {
    case numeric_type::int8:
    case numeric_type::int16:
    case numeric_type::int32:
    case numeric_type::int64:
        return true;
    default:
        return false;
    }
}

XMIPP4_INLINE_CONSTEXPR bool is_uint(numeric_type type) noexcept
{
    switch (type)
    {
    case numeric_type::uint8:
    case numeric_type::uint16:
    case numeric_type::uint32:
    case numeric_type::uint64:
        return true;
    default:
        return false;
    }
}

XMIPP4_INLINE_CONSTEXPR bool is_int_or_uint(numeric_type type) noexcept
{
    return is_int(type) || is_uint(type);
}

XMIPP4_INLINE_CONSTEXPR bool is_float(numeric_type type) noexcept
{
    switch (type)
    {
    case numeric_type::float16:
    case numeric_type::float32:
    case numeric_type::float64:
    case numeric_type::bfloat16:
    case numeric_type::bfloat19:
        return true;
    default:
        return false;
    }
}

XMIPP4_INLINE_CONSTEXPR bool is_complex(numeric_type type) noexcept
{
    switch (type)
    {
    case numeric_type::complex_float16:
    case numeric_type::complex_float32:
    case numeric_type::complex_float64:
        return true;
    default:
        return false;
    }
}



XMIPP4_INLINE_CONSTEXPR const char*
to_string(numeric_type type) noexcept
{
    switch (type)
    {
    case numeric_type::int8: return "int8";
    case numeric_type::int16: return "int16";
    case numeric_type::int32: return "int32";
    case numeric_type::int64: return "int64";
    case numeric_type::uint8: return "uint8";
    case numeric_type::uint16: return "uint16";
    case numeric_type::uint32: return "uint32";
    case numeric_type::uint64: return "uint64";
    case numeric_type::float16: return "float16";
    case numeric_type::float32: return "float32";
    case numeric_type::float64: return "float64";
    case numeric_type::bfloat16: return "bfloat16";
    case numeric_type::bfloat19: return "bfloat19";
    case numeric_type::complex_float16: return "complex_float16";
    case numeric_type::complex_float32: return "complex_float32";
    case numeric_type::complex_float64: return "complex_float64";
    }
}


inline numeric_type 
from_string(std::string_view str)
{
    static const 
    std::unordered_map<std::string_view, numeric_type> str_2_numeric_type = 
    {
        { to_string(numeric_type::int8), numeric_type::int8 },
        { to_string(numeric_type::int16), numeric_type::int16 },
        { to_string(numeric_type::int32), numeric_type::int32 },
        { to_string(numeric_type::int64), numeric_type::int64 },
        { to_string(numeric_type::uint8), numeric_type::uint8 },
        { to_string(numeric_type::uint16), numeric_type::uint16 },
        { to_string(numeric_type::uint32), numeric_type::uint32 },
        { to_string(numeric_type::uint64), numeric_type::uint64 },
        { to_string(numeric_type::float16), numeric_type::float16 },
        { to_string(numeric_type::float32), numeric_type::float32 },
        { to_string(numeric_type::float64), numeric_type::float64 },
        { to_string(numeric_type::bfloat16), numeric_type::bfloat16 },
        { to_string(numeric_type::bfloat19), numeric_type::bfloat19 },
        { to_string(numeric_type::complex_float16), numeric_type::complex_float16 },
        { to_string(numeric_type::complex_float32), numeric_type::complex_float32 },
        { to_string(numeric_type::complex_float64), numeric_type::complex_float64 },
    };

    return str_2_numeric_type.at(str);
}

}
