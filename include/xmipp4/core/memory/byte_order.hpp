// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file byte_order.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of byte_order dependant words and conversion methods
 * @date 2023-08-12
 * 
 */

#include "../platform/attributes.hpp"
#include "../platform/constexpr.hpp"

#include <type_traits>

namespace xmipp4
{
namespace memory
{

enum class byte_order 
{
    big_endian,
    little_endian,
    //pdp_endian, //unsupported
    //honeywell_endian, //unsupported
};

XMIPP4_CONSTEXPR byte_order get_system_byte_order() noexcept;
XMIPP4_CONSTEXPR byte_order get_fpu_byte_order() noexcept;

template<typename T>
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T>::type
reverse_byte_order(T x) noexcept;

template<typename T>
XMIPP4_CONSTEXPR T& reverse_byte_order_inplace(T& x) noexcept;

template<byte_order From, byte_order To, typename T>
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T>::type
convert_byte_order(T x) noexcept;

template<typename T>
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T>::type
convert_byte_order(T x, byte_order from, byte_order to) noexcept;

template<byte_order From, byte_order To, typename T>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T&>::type
convert_byte_order_inplace(T& x) noexcept;

template<typename T>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<T>::value, T&>::type
convert_byte_order_inplace(T& x, byte_order from, byte_order to) noexcept;

} // namespace memory
} // namespace xmipp4

#include "byte_order.inl"
