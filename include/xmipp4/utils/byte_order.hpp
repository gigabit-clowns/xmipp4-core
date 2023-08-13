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
 * @file byte_order.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of byte_order dependant words and conversion methods
 * @date 2023-08-12
 * 
 */

#include "../platform/attributes.hpp"
#include "../platform/constexpr.hpp"
#include "../numeric_type.hpp"

#include <type_traits>

namespace xmipp4
{
namespace utils
{

enum class byte_order 
{
    big_endian,
    little_endian,
    //pdp_endian, //unsupported
    //honeywell_endian, //unsupported
};

template<typename T, byte_order O>
class fixed_byte_order
{
public:
    using value_type = T;
    static inline XMIPP4_CONST_CONSTEXPR byte_order order = O;

    fixed_byte_order() = default;
    XMIPP4_CONSTEXPR fixed_byte_order(value_type value) noexcept;
    fixed_byte_order(const fixed_byte_order& other) = default;
    fixed_byte_order(fixed_byte_order&& other) = default;
    ~fixed_byte_order() = default;
    
    fixed_byte_order& operator=(const fixed_byte_order& other) = default;
    fixed_byte_order& operator=(fixed_byte_order&& other) = default;

    XMIPP4_CONSTEXPR operator value_type() const noexcept;

private:
    value_type m_value;

};

using big_int8_t = int8_t;
using little_int8_t = int8_t;
using big_uint8_t = uint8_t;
using little_uint8_t = uint8_t;
using big_int16_t = fixed_byte_order<int16_t, byte_order::big_endian>;
using little_int16_t = fixed_byte_order<int16_t, byte_order::little_endian>;
using big_uint16_t = fixed_byte_order<uint16_t, byte_order::big_endian>;
using little_uint16_t = fixed_byte_order<uint16_t, byte_order::little_endian>;
using big_int32_t = fixed_byte_order<int32_t, byte_order::big_endian>;
using little_int32_t = fixed_byte_order<int32_t, byte_order::little_endian>;
using big_uint32_t = fixed_byte_order<uint32_t, byte_order::big_endian>;
using little_uint32_t = fixed_byte_order<uint32_t, byte_order::little_endian>;
using big_int64_t = fixed_byte_order<int64_t, byte_order::big_endian>;
using little_int64_t = fixed_byte_order<int64_t, byte_order::little_endian>;
using big_uint64_t = fixed_byte_order<uint64_t, byte_order::big_endian>;
using little_uint64_t = fixed_byte_order<uint64_t, byte_order::little_endian>;
using big_float16_t = fixed_byte_order<float16_t, byte_order::big_endian>;
using little_float16_t = fixed_byte_order<float16_t, byte_order::little_endian>;
using big_float32_t = fixed_byte_order<float32_t, byte_order::big_endian>;
using little_float32_t = fixed_byte_order<float32_t, byte_order::little_endian>;
using big_float64_t = fixed_byte_order<float64_t, byte_order::big_endian>;
using little_float64_t = fixed_byte_order<float64_t, byte_order::little_endian>;


XMIPP4_CONSTEXPR byte_order get_system_byte_order() noexcept;

XMIPP4_CONSTEXPR byte_order get_fpu_byte_order() noexcept;

template <typename T>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_scalar<T>::value, byte_order>::type
get_native_byte_order() noexcept;

template<typename T>
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_scalar<T>::value, T>::type
reverse_byte_order(T x) noexcept;

template<typename T>
XMIPP4_CONSTEXPR T& reverse_byte_order_inplace(T& x) noexcept;

template<byte_order From, byte_order To, typename T>
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_scalar<T>::value, T>::type
convert_byte_order(T x) noexcept;

template<typename T>
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_scalar<T>::value, T>::type
convert_byte_order(T x, byte_order from, byte_order to) noexcept;

template<byte_order From, byte_order To, typename T>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_scalar<T>::value, T&>::type
convert_byte_order_inplace(T& x) noexcept;

template<typename T>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_scalar<T>::value, T&>::type
convert_byte_order_inplace(T& x, byte_order from, byte_order to) noexcept;

} // namespace utils
} // namespace xmipp4

#include "byte_order.inl"