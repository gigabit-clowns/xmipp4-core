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
 * @file version.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of the version class
 * @date 2023-08-12
 * 
 */

#include "platform/constexpr.hpp"
#include "utils/bit.hpp"

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
        utils::bit_range_mask<std::uint32_t>(patch_offset, patch_offset+patch_bits);
    static XMIPP4_INLINE_CONST_CONSTEXPR std::uint32_t minor_mask = 
        utils::bit_range_mask<std::uint32_t>(minor_offset, minor_offset+minor_bits);
    static XMIPP4_INLINE_CONST_CONSTEXPR std::uint32_t major_mask = 
        utils::bit_range_mask<std::uint32_t>(major_offset, major_offset+major_bits);

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
