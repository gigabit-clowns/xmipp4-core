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
 * @file byte.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Provides C++17 like byte type
 * @date 2023-08-12
 * 
 */

#include "../platform/attributes.hpp"
#include "../platform/constexpr.hpp"

#include <type_traits>
#include <cstdint>
#include <functional>

namespace xmipp4
{
namespace memory
{

enum class byte : uint8_t {};

XMIPP4_CONSTEXPR uint8_t as_uint8(byte b) noexcept;
XMIPP4_CONSTEXPR byte as_byte(uint8_t b) noexcept;

template <typename T>
byte* as_bytes(T* ptr) noexcept;

template <typename T>
const byte* as_bytes(const T* ptr) noexcept;

template <class IntegerType>
XMIPP4_CONSTEXPR byte operator<<(byte b, IntegerType shift) noexcept;
template <class IntegerType>
XMIPP4_CONSTEXPR byte operator>>(byte b, IntegerType shift) noexcept;
template <class IntegerType>
XMIPP4_CONSTEXPR byte& operator<<=(byte& b, IntegerType shift) noexcept;
template <class IntegerType>
XMIPP4_CONSTEXPR byte& operator>>=(byte& b, IntegerType shift) noexcept;

XMIPP4_CONSTEXPR byte operator~(byte b) noexcept;
XMIPP4_CONSTEXPR byte operator|(byte lhs, byte rhs) noexcept;
XMIPP4_CONSTEXPR byte operator&(byte lhs, byte rhs) noexcept;
XMIPP4_CONSTEXPR byte operator^(byte lhs, byte rhs) noexcept;
XMIPP4_CONSTEXPR byte& operator|=(byte& lhs, byte rhs) noexcept;
XMIPP4_CONSTEXPR byte& operator&=(byte& lhs, byte rhs) noexcept;
XMIPP4_CONSTEXPR byte& operator^=(byte& lhs, byte rhs) noexcept;

XMIPP4_CONSTEXPR std::size_t get_byte_bits() noexcept;

template <typename C>
XMIPP4_CONSTEXPR void to_hex(byte b, C &high, C &low) noexcept;

} // namespace memory
} // namespace xmipp4

template <>
struct std::hash<xmipp4::memory::byte>
{
    XMIPP4_CONSTEXPR size_t operator()(xmipp4::memory::byte b) const noexcept;
};

#include "byte.inl"
