// SPDX-License-Identifier: GPL-3.0-only

#pragma once

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

/**
 * @brief Offset an address by a given amount of bytes
 * 
 * @tparam T Pointed type.
 * @param address Pointer to be modified.
 * @param count Number of bytes to add.
 * @return T* Pointer with offset.
 * 
 * @warning This may break alignment requirements. Of the underlying type.
 * Use it under your own risk 
 * 
 */
template <typename T>
XMIPP4_NODISCARD
T* offset_bytes(T* address, std::ptrdiff_t count);

} // namespace memory
} // namespace xmipp4

#include "align.inl"
