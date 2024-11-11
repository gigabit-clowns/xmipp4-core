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

#include "align.hpp"

#include "../utils/bit.hpp"

namespace xmipp4
{
namespace memory
{

template <typename T>
XMIPP4_NODISCARD inline 
std::size_t get_alignment(T* address) noexcept
{
    return get_alignment(reinterpret_cast<std::uintptr_t>(address));
}

XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
std::size_t get_alignment(std::uintptr_t address) noexcept
{
    return utils::mask_trailing_zeros(address) + 1;
}

template <typename T>
XMIPP4_NODISCARD inline
T* align_floor(T* address, std::size_t alignment) noexcept
{
    align_floor_inplace(reinterpret_cast<std::uintptr_t&>(address), alignment);
    return address;
}

XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
std::uintptr_t align_floor(std::uintptr_t address, std::size_t alignment) noexcept
{
    align_floor_inplace(address, alignment);
    return address;
}

XMIPP4_INLINE_CONSTEXPR 
std::uintptr_t& align_floor_inplace(std::uintptr_t& address, std::size_t alignment) noexcept
{
    address /= alignment;
    address *= alignment;
    return address;
}

template <typename T>
XMIPP4_NODISCARD inline
T* align_ceil(T* address, std::size_t alignment) noexcept
{
    align_ceil_inplace(reinterpret_cast<std::uintptr_t&>(address), alignment);
    return address;
}

XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
std::uintptr_t align_ceil(std::uintptr_t address, std::size_t alignment) noexcept
{
    align_ceil_inplace(address, alignment);
    return address;
}

XMIPP4_INLINE_CONSTEXPR 
std::uintptr_t& align_ceil_inplace(std::uintptr_t& address, std::size_t alignment) noexcept
{
    address += (alignment - 1);
    return align_floor_inplace(address, alignment);
}

template <typename T>
XMIPP4_NODISCARD inline
T* offset_bytes(T* address, std::ptrdiff_t count)
{
    auto &value = reinterpret_cast<std::uintptr_t&>(address);
    value += count;
    return address;
}

} // namespace memory
} // namespace xmipp4
