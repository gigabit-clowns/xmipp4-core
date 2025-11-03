// SPDX-License-Identifier: GPL-3.0-only

#include "align.hpp"

#include "../binary/bit.hpp"

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
    return binary::mask_trailing_zeros(address) + 1;
}

template <typename T>
XMIPP4_NODISCARD inline
bool is_aligned(T* address, std::size_t alignment) noexcept
{
    return is_aligned(reinterpret_cast<std::uintptr_t>(address), alignment);
}

XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
bool is_aligned(std::uintptr_t address, std::size_t alignment) noexcept
{
    return address % alignment == 0;
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
    auto value = reinterpret_cast<std::uintptr_t>(address);
    value += count;
    return reinterpret_cast<T*>(value);
}

} // namespace memory
} // namespace xmipp4
