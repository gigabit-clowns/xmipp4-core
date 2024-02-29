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
 * @file byte.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of byte.hpp
 * @date 2023-08-12
 * 
 */

#include "byte.hpp"

#include <climits>

namespace xmipp4
{
namespace utils
{

XMIPP4_INLINE_CONSTEXPR uint8_t 
as_uint8(byte b) noexcept
{
    return static_cast<uint8_t>(b);
}


XMIPP4_INLINE_CONSTEXPR byte 
as_byte(uint8_t b) noexcept
{
    return static_cast<byte>(b);
}

template <typename T>
inline byte* 
as_bytes(T* ptr) noexcept
{
    return reinterpret_cast<byte*>(ptr);
}

template <typename T>
inline const byte* 
as_bytes(const T* ptr) noexcept
{
    return reinterpret_cast<const byte*>(ptr);
}



template <class IntegerType>
XMIPP4_INLINE_CONSTEXPR byte 
operator<<(byte b, IntegerType shift) noexcept
{
    return as_byte(as_uint8(b) << shift);
}

template <class IntegerType>
XMIPP4_INLINE_CONSTEXPR byte 
operator>>(byte b, IntegerType shift) noexcept
{
    return as_byte(as_uint8(b) >> shift);
}

template <class IntegerType>
XMIPP4_INLINE_CONSTEXPR byte& 
operator<<=(byte& b, IntegerType shift) noexcept
{
    b = b << shift;
    return b;
}

template <class IntegerType>
XMIPP4_INLINE_CONSTEXPR byte& 
operator>>=(byte& b, IntegerType shift) noexcept
{
    b = b >> shift;
    return b;
}

XMIPP4_INLINE_CONSTEXPR byte 
operator~(byte b) noexcept
{
    return as_byte(~as_uint8(b));
}

XMIPP4_INLINE_CONSTEXPR byte 
operator|(byte lhs, byte rhs) noexcept
{
    return as_byte(as_uint8(lhs) | as_uint8(rhs));
}

XMIPP4_INLINE_CONSTEXPR byte 
operator&(byte lhs, byte rhs) noexcept
{
    return as_byte(as_uint8(lhs) & as_uint8(rhs));
}

XMIPP4_INLINE_CONSTEXPR byte 
operator^(byte lhs, byte rhs) noexcept
{
    return as_byte(as_uint8(lhs) ^ as_uint8(rhs));
}

XMIPP4_INLINE_CONSTEXPR byte& 
operator|=(byte& lhs, byte rhs) noexcept
{
    lhs = lhs | rhs;
    return lhs;
}

XMIPP4_INLINE_CONSTEXPR byte& 
operator&=(byte& lhs, byte rhs) noexcept
{
    lhs = lhs & rhs;
    return lhs;
}

XMIPP4_INLINE_CONSTEXPR byte& 
operator^=(byte& lhs, byte rhs) noexcept
{
    lhs = lhs ^ rhs;
    return lhs;
}



XMIPP4_INLINE_CONSTEXPR std::size_t 
get_byte_bits() noexcept
{
    #if defined(CHAR_BIT)
        return CHAR_BIT;
    #else
        #pragma warning "Could not determine byte size. Defaulting to 8"
        return 8UL;
    #endif
}

} // namespace utils
} // namespace xmipp4

XMIPP4_INLINE_CONSTEXPR size_t 
std::hash<xmipp4::utils::byte>::operator()(xmipp4::utils::byte b) const noexcept
{
    return xmipp4::utils::as_uint8(b);
}