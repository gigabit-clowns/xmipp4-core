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
 * @file align.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Utility functions related to memory address manipulations
 * @date 2023-08-31
 * 
 */

#include "../platform/attributes.hpp"
#include "../platform/constexpr.hpp"

#include <cstdint>

namespace xmipp4
{
namespace memory
{

/**
 * @brief Get the alignment of a pointer
 * 
 * Finds the largest power of two that is a divisor of the address
 * 
 * @tparam T Pointed value type
 * @param address The address of which the alignment is determined
 * @return std::size_t The alignment (a power of two)
 */
template <typename T>
XMIPP4_NODISCARD 
std::size_t get_alignment(T* address) noexcept;

/**
 * @brief Get the alignment of an address
 * 
 * Finds the largest power of two that is a divisor of the address
 * 
 * @param address The address of which the alignment is determined
 * @return std::size_t The alignment (a power of two)
 */
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
std::size_t get_alignment(std::uintptr_t address) noexcept;

/**
 * @brief Align a pointer to a lower or equal address that satisfies the
 * provided alignment
 * 
 * @tparam T Pointed value type
 * @param address The address to be aligned
 * @param alignment The minimum alignment
 * @return T* Aligned pointer
 */
template <typename T>
XMIPP4_NODISCARD 
T* align_floor(T* address, std::size_t alignment) noexcept;

/**
 * @brief Align an address to a lower or equal address that satisfies the
 * provided alignment
 * 
 * @param address The address to be aligned
 * @param alignment The minimum alignment
 * @return std::uintptr_t Aligned address
 */
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
std::uintptr_t align_floor(std::uintptr_t address, std::size_t alignment) noexcept;

/**
 * @brief Align an address to a lower or equal address that satisfies the
 * provided alignment
 * 
 * @param address The address to be aligned
 * @param alignment The minimum alignment
 * @return std::uintptr_t Aligned address
 * @note The result is written in place and a reference is returned
 */
XMIPP4_CONSTEXPR 
std::uintptr_t& align_floor_inplace(std::uintptr_t& address, std::size_t alignment) noexcept;

/**
 * @brief Align a pointer to a higher or equal address that satisfies the
 * provided alignment
 * 
 * @tparam T Pointed value type
 * @param address The address to be aligned
 * @param alignment The minimum alignment
 * @return T* Aligned pointer
 */

template <typename T>
XMIPP4_NODISCARD 
T* align_ceil(T* address, std::size_t alignment) noexcept;

/**
 * @brief Align an address to a higher or equal address that satisfies the
 * provided alignment
 * 
 * @param address The address to be aligned
 * @param alignment The minimum alignment
 * @return std::uintptr_t Aligned address
 */
XMIPP4_NODISCARD XMIPP4_CONSTEXPR 
std::uintptr_t align_ceil(std::uintptr_t address, std::size_t alignment) noexcept;

/**
 * @brief Align an address to a higher or equal address that satisfies the
 * provided alignment
 * 
 * @param address The address to be aligned
 * @param alignment The minimum alignment
 * @return std::uintptr_t Aligned address
 * @note The result is written in place and a reference is returned
 */
XMIPP4_CONSTEXPR 
std::uintptr_t& align_ceil_inplace(std::uintptr_t& address, std::size_t alignment) noexcept;

} // namespace memory
} // namespace xmipp4

#include "align.inl"
