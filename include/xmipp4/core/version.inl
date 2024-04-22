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
 * @file version.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of version.hpp
 * @date 2023-08-12
 * 
 */

#include "version.hpp"

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR 
version::version(std::uint32_t major, 
                 std::uint32_t minor, 
                 std::uint32_t patch ) noexcept
    : m_data(major) // No need to mask, as it will be shifted
{
    // Shift and write fields
    XMIPP4_CONST_CONSTEXPR std::uint32_t minor_mask = (1 << minor_bits) - 1;
    m_data <<= minor_bits;
    m_data |= minor & minor_mask;

    XMIPP4_CONST_CONSTEXPR std::uint32_t patch_mask = (1 << patch_bits) - 1;
    m_data <<= patch_bits;
    m_data |= patch & patch_mask;
}

XMIPP4_INLINE_CONSTEXPR 
bool version::operator==(const version& other) noexcept
{
    return m_data == other.m_data;
}

XMIPP4_INLINE_CONSTEXPR 
bool version::operator!=(const version& other) noexcept
{
    return m_data != other.m_data;
}

XMIPP4_INLINE_CONSTEXPR 
bool version::operator<(const version& other) noexcept
{
    return m_data < other.m_data;
}

XMIPP4_INLINE_CONSTEXPR 
bool version::operator<=(const version& other) noexcept
{
    return m_data <= other.m_data;
}

XMIPP4_INLINE_CONSTEXPR 
bool version::operator>(const version& other) noexcept
{
    return m_data > other.m_data;
}

XMIPP4_INLINE_CONSTEXPR 
bool version::operator>=(const version& other) noexcept
{
    return m_data >= other.m_data;
}

XMIPP4_INLINE_CONSTEXPR 
void version::set_major(std::uint32_t major) noexcept
{
    major <<= major_offset;
    m_data &= ~major_mask;
    m_data |= major;
}

XMIPP4_INLINE_CONSTEXPR 
std::uint32_t version::get_major() const noexcept
{
    return m_data >> major_offset;
}

XMIPP4_INLINE_CONSTEXPR 
void version::set_minor(std::uint32_t minor) noexcept
{
    minor <<= minor_offset;
    minor &= minor_mask;
    m_data &= ~minor_mask;
    m_data |= minor;
}

XMIPP4_INLINE_CONSTEXPR 
std::uint32_t version::get_minor() const noexcept
{
    return (m_data & minor_mask) >> minor_bits;
}

XMIPP4_INLINE_CONSTEXPR 
void version::set_patch(std::uint32_t patch) noexcept
{
    patch &= patch_mask;
    m_data &= ~patch_mask;
    m_data |= patch;
}

XMIPP4_INLINE_CONSTEXPR 
std::uint32_t version::get_patch() const noexcept
{
    return m_data & patch_mask;
}



template<typename T>
inline std::basic_ostream<T>& 
operator<<(std::basic_ostream<T>& os, const version& ver)
{
    XMIPP4_CONST_CONSTEXPR T sep = '.';
    return os   << ver.get_major() << sep 
                << ver.get_minor() << sep
                << ver.get_patch();
}

} // namespace xmipp4
