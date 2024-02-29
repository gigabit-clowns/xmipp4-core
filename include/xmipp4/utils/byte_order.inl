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
 * @file byte_order.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of byte_order.hpp
 * @date 2023-08-12
 * 
 */

#include "byte_order.hpp"

#include "../platform/byte_order.h"
#include "../platform/builtin.h"

#include <cstdint>

namespace xmipp4
{
namespace utils
{

XMIPP4_INLINE_CONSTEXPR byte_order 
get_system_byte_order() noexcept
{
    #if XMIPP4_BYTE_ORDER == XMIPP4_BYTE_ORDER_LITTLE_ENDIAN
        return byte_order::little_endian;
    #elif XMIPP4_BYTE_ORDER == XMIPP4_BYTE_ORDER_BIG_ENDIAN
        return byte_order::big_endian;
    #else
        #error "Unknown byte ordering"
    #endif
}

XMIPP4_INLINE_CONSTEXPR byte_order 
get_fpu_byte_order() noexcept
{
    #if XMIPP4_FLOAT_BYTE_ORDER == XMIPP4_BYTE_ORDER_LITTLE_ENDIAN
        return byte_order::little_endian;
    #elif XMIPP4_FLOAT_BYTE_ORDER == XMIPP4_BYTE_ORDER_BIG_ENDIAN
        return byte_order::big_endian;
    #else
        #error "Unknown byte ordering"
    #endif
}



namespace detail
{

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, uint8_t>::value, T>::type
reverse_byte_order(T x) noexcept
{
    return x;
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, int8_t>::value, T>::type
reverse_byte_order(T x) noexcept
{
    return x;
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, uint16_t>::value, T>::type
reverse_byte_order(T x) noexcept
{
#if XMIPP4_HAS_BUILTIN(bswap16)
    return XMIPP4_BUILTIN(bswap16)(x); 
#else
    return  (x << 8) | 
            (x >> 8) ;
#endif
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, int16_t>::value, T>::type
reverse_byte_order(T x) noexcept
{
    return reverse_byte_order(uint16_t(x));
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, uint32_t>::value, T>::type
reverse_byte_order(T x) noexcept
{
#if XMIPP4_HAS_BUILTIN(bswap32)
    return XMIPP4_BUILTIN(bswap32)(x); 
#else
    return  (x>>24) |
            ((x<<8) & 0x00FF0000) |
            ((x>>8) & 0x0000FF00) |
            (x<<24) ;
#endif
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, int32_t>::value, T>::type
reverse_byte_order(T x) noexcept
{
    return reverse_byte_order(uint32_t(x));
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, uint64_t>::value, T>::type
reverse_byte_order(T x) noexcept
{
#if XMIPP4_HAS_BUILTIN(bswap64)
    return XMIPP4_BUILTIN(bswap64)(x); 
#else
    return  (x>>56) |
            ((x<<40) & 0x00FF000000000000) |
            ((x<<24) & 0x0000FF0000000000) |
            ((x<<8)  & 0x000000FF00000000) |
            ((x>>8)  & 0x00000000FF000000) |
            ((x>>24) & 0x0000000000FF0000) |
            ((x>>40) & 0x000000000000FF00) |
            (x<<56) ;
#endif
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_same<T, int64_t>::value, T>::type
reverse_byte_order(T x) noexcept
{
    return reverse_byte_order(uint64_t(x));
}

} // namespace detail

template<typename T>
XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T>::type
reverse_byte_order(T x) noexcept
{
    return detail::reverse_byte_order(x);
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR T& reverse_byte_order_inplace(T& x) noexcept
{
    return x = reverse_byte_order(x);
}



template<byte_order From, byte_order To, typename T>
XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T>::type
convert_byte_order(T x) noexcept
{
    return convert_byte_order(x, From, To);
}

template<typename T>
XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T>::type
convert_byte_order(T x, byte_order from, byte_order to) noexcept
{
    convert_byte_order_inplace(x, from, to);
    return x;
}

template<byte_order From, byte_order To, typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T&>::type
convert_byte_order_inplace(T& x) noexcept
{
    return convert_byte_order_inplace(x, From, To);
}

template<typename T>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T&>::type
convert_byte_order_inplace(T& x, byte_order from, byte_order to) noexcept
{
    switch (from)
    {
    case byte_order::big_endian:
        switch (to)
        {
        case byte_order::big_endian:
            return x;
        case byte_order::little_endian:
            return reverse_byte_order_inplace(x);
        }

    case byte_order::little_endian:
        switch (to)
        {
        case byte_order::big_endian:
            return reverse_byte_order_inplace(x);
        case byte_order::little_endian:
            return x;
        }
    }
}

} // namespace utils
} // namespace xmipp4
