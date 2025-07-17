// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "platform/constexpr.hpp"
#include "binary/bit.hpp"

#include <ostream>
#include <cstdint>

namespace xmipp4 
{

class version
{
public:
    version() = default;
    XMIPP4_CONSTEXPR version(std::uint32_t major, 
                             std::uint32_t minor, 
                             std::uint32_t patch ) noexcept;
    version(const version& other) = default;
    ~version() = default;

    version& operator=(const version& other) = default;

    XMIPP4_CONSTEXPR void set_major(std::uint32_t major) noexcept;
    XMIPP4_CONSTEXPR std::uint32_t get_major() const noexcept;

    XMIPP4_CONSTEXPR void set_minor(std::uint32_t minor) noexcept;
    XMIPP4_CONSTEXPR std::uint32_t get_minor() const noexcept;

    XMIPP4_CONSTEXPR void set_patch(std::uint32_t patch) noexcept;
    XMIPP4_CONSTEXPR std::uint32_t get_patch() const noexcept;

    XMIPP4_CONSTEXPR std::uint32_t get_data() const noexcept;

private:
    static XMIPP4_INLINE_CONST_CONSTEXPR std::size_t patch_bits = 10;
    static XMIPP4_INLINE_CONST_CONSTEXPR std::size_t minor_bits = 10;
    static XMIPP4_INLINE_CONST_CONSTEXPR std::size_t major_bits = 12;

    static XMIPP4_INLINE_CONST_CONSTEXPR std::size_t patch_offset = 0;
    static XMIPP4_INLINE_CONST_CONSTEXPR std::size_t minor_offset = patch_offset + patch_bits;
    static XMIPP4_INLINE_CONST_CONSTEXPR std::size_t major_offset = minor_offset + minor_bits;

    static XMIPP4_INLINE_CONST_CONSTEXPR std::uint32_t patch_mask = 
        binary::bit_range_mask<std::uint32_t>(patch_offset, patch_offset+patch_bits);
    static XMIPP4_INLINE_CONST_CONSTEXPR std::uint32_t minor_mask = 
        binary::bit_range_mask<std::uint32_t>(minor_offset, minor_offset+minor_bits);
    static XMIPP4_INLINE_CONST_CONSTEXPR std::uint32_t major_mask = 
        binary::bit_range_mask<std::uint32_t>(major_offset, major_offset+major_bits);

    std::uint32_t m_data;

};

XMIPP4_CONSTEXPR bool operator==(const version& lhs, const version& rhs) noexcept;
XMIPP4_CONSTEXPR bool operator!=(const version& lhs, const version& rhs) noexcept;
XMIPP4_CONSTEXPR bool operator<(const version& lhs, const version& rhs) noexcept;
XMIPP4_CONSTEXPR bool operator<=(const version& lhs, const version& rhs) noexcept;
XMIPP4_CONSTEXPR bool operator>(const version& lhs, const version& rhs) noexcept;
XMIPP4_CONSTEXPR bool operator>=(const version& lhs, const version& rhs) noexcept;
    
template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, const version& ver);

} // namespace xmipp4

#include "version.inl"
