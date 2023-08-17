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
 * @file numeric_type.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of supported numeric types
 * @date 2023-08-12
 * 
 */

#include "platform/constexpr.hpp"

#include <cstdint>
#include <complex>
#include <typeindex>
#include <string_view>

namespace xmipp4
{

using int8_t = ::int8_t;
using int16_t = ::int16_t;
using int32_t = ::int32_t;
using int64_t = ::int64_t;

using uint8_t = ::uint8_t;
using uint16_t = ::uint16_t;
using uint32_t = ::uint32_t;
using uint64_t = ::uint64_t;


#if __HAVE_FLOAT16
using float16_t = _Float16;
#else
struct float16_t { uint16_t bits; };
#endif

#if __HAVE_FLOAT32
using float32_t = _Float32;
#else
#warning "_Float32 is not defined. Defaulting to float"
using float32_t = float;
#endif

#if __HAVE_FLOAT64
using float64_t = _Float64;
#else
#warning "_Float64 is not defined. Defaulting to double"
using float64_t = double;
#endif

using complex_float16_t = std::complex<float16_t>;
using complex_float32_t = std::complex<float32_t>;
using complex_float64_t = std::complex<float64_t>;

enum class numeric_type
{
    int8,
    int16,
    int32,
    int64,

    uint8,
    uint16,
    uint32,
    uint64,

    float16,
    float32,
    float64,

    complex_float16,
    complex_float32,
    complex_float64
};

template <typename T>
XMIPP4_CONSTEXPR numeric_type type_of() noexcept;
template <typename T>
XMIPP4_CONSTEXPR numeric_type type_of(const T&) noexcept;

XMIPP4_CONSTEXPR std::size_t get_sizeof(numeric_type type) noexcept;
XMIPP4_CONSTEXPR std::size_t get_alignof(numeric_type type) noexcept;
XMIPP4_CONSTEXPR const std::type_info& get_typeid(numeric_type type) noexcept;
XMIPP4_CONSTEXPR bool is_int(numeric_type type) noexcept;
XMIPP4_CONSTEXPR bool is_uint(numeric_type type) noexcept;
XMIPP4_CONSTEXPR bool is_int_or_uint(numeric_type type) noexcept;
XMIPP4_CONSTEXPR bool is_float(numeric_type type) noexcept;
XMIPP4_CONSTEXPR bool is_complex(numeric_type type) noexcept;

XMIPP4_CONSTEXPR const char* to_string(numeric_type type) noexcept;
numeric_type from_string(std::string_view str);

}

#include "numeric_type.inl"
