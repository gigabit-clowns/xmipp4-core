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
