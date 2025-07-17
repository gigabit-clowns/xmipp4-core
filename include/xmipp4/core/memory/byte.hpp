// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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
#include <iostream>

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

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, const byte& b);

} // namespace memory
} // namespace xmipp4

template <>
struct std::hash<xmipp4::memory::byte>
{
    XMIPP4_CONSTEXPR size_t operator()(xmipp4::memory::byte b) const noexcept;
};

#include "byte.inl"
