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

#include "../platform/attributes.hpp"
#include "../platform/constexpr.hpp"

#include <cstdint>

namespace xmipp4
{
namespace utils
{

template <typename T>
XMIPP4_NODISCARD std::size_t get_alignment(T* address) noexcept;
XMIPP4_NODISCARD XMIPP4_CONSTEXPR std::size_t get_alignment(std::uintptr_t address) noexcept;

template <typename T>
XMIPP4_NODISCARD T* align_floor(T* address, std::size_t alignment) noexcept;
XMIPP4_NODISCARD XMIPP4_CONSTEXPR std::uintptr_t align_floor(std::uintptr_t address, std::size_t alignment) noexcept;
XMIPP4_CONSTEXPR std::uintptr_t& align_floor_inplace(std::uintptr_t& address, std::size_t alignment) noexcept;

template <typename T>
XMIPP4_NODISCARD T* align_ceil(T* address, std::size_t alignment) noexcept;
XMIPP4_NODISCARD XMIPP4_CONSTEXPR std::uintptr_t align_ceil(std::uintptr_t address, std::size_t alignment) noexcept;
XMIPP4_CONSTEXPR std::uintptr_t& align_ceil_inplace(std::uintptr_t& address, std::size_t alignment) noexcept;

} // namespace utils
} // namespace xmipp4

#include "memory.inl"

