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

#include <tuple>
#include <ostream>
#include <cstdint>

#define XMIPP4_VERSION_MAJOR_BITS 4
#define XMIPP4_VERSION_MINOR_BITS 14
#define XMIPP4_VERSION_PATCH_BITS 14

namespace xmipp4 
{

class version
{
public:
    version() = default;
    XMIPP4_CONSTEXPR version(   unsigned major, 
                                unsigned minor, 
                                unsigned patch ) noexcept;
    version(const version& other) = default;
    ~version() = default;

    version& operator=(const version& other) = default;
    
    XMIPP4_CONSTEXPR void set_major(unsigned major) noexcept;
    XMIPP4_CONSTEXPR unsigned get_major() const noexcept;

    XMIPP4_CONSTEXPR void set_minor(unsigned minor) noexcept;
    XMIPP4_CONSTEXPR unsigned get_minor() const noexcept;

    XMIPP4_CONSTEXPR void set_patch(unsigned patch) noexcept;
    XMIPP4_CONSTEXPR unsigned get_patch() const noexcept;

private:
    uint32_t m_major : XMIPP4_VERSION_MAJOR_BITS;
    uint32_t m_minor : XMIPP4_VERSION_MINOR_BITS;
    uint32_t m_patch : XMIPP4_VERSION_PATCH_BITS;

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
