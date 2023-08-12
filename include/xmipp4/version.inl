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

XMIPP4_INLINE_CONSTEXPR version::version( unsigned major, 
                            unsigned minor, 
                            unsigned patch ) noexcept
    : m_major(major)
    , m_minor(minor)
    , m_patch(patch)
{
}

XMIPP4_INLINE_CONSTEXPR void 
version::set_major(unsigned major) noexcept
{
    m_major = major;
}

XMIPP4_INLINE_CONSTEXPR unsigned 
version::get_major() const noexcept
{
    return m_major;
}

XMIPP4_INLINE_CONSTEXPR void 
version::set_minor(unsigned minor) noexcept
{
    m_minor = minor;
}

XMIPP4_INLINE_CONSTEXPR unsigned 
version::get_minor() const noexcept
{
    return m_minor;
}

XMIPP4_INLINE_CONSTEXPR void 
version::set_patch(unsigned patch) noexcept
{
    m_patch = patch;
}

XMIPP4_INLINE_CONSTEXPR unsigned 
version::get_patch() const noexcept
{
    return m_patch;
}



namespace detail
{

XMIPP4_INLINE_CONSTEXPR std::tuple<unsigned, unsigned, unsigned>
as_tuple(const version& version) noexcept
{
    return std::make_tuple(
        version.get_major(),
        version.get_minor(),
        version.get_patch()
    );
}

}

XMIPP4_INLINE_CONSTEXPR bool 
operator==(const version& lhs, const version& rhs) noexcept
{
    return detail::as_tuple(lhs) == detail::as_tuple(rhs);
}

XMIPP4_INLINE_CONSTEXPR bool 
operator!=(const version& lhs, const version& rhs) noexcept
{
    return detail::as_tuple(lhs) != detail::as_tuple(rhs);
}

XMIPP4_INLINE_CONSTEXPR bool 
operator<(const version& lhs, const version& rhs) noexcept
{
    return detail::as_tuple(lhs) < detail::as_tuple(rhs);
}

XMIPP4_INLINE_CONSTEXPR bool 
operator<=(const version& lhs, const version& rhs) noexcept
{
    return detail::as_tuple(lhs) <= detail::as_tuple(rhs);
}

XMIPP4_INLINE_CONSTEXPR bool 
operator>(const version& lhs, const version& rhs) noexcept
{
    return detail::as_tuple(lhs) > detail::as_tuple(rhs);
}

XMIPP4_INLINE_CONSTEXPR bool 
operator>=(const version& lhs, const version& rhs) noexcept
{
    return detail::as_tuple(lhs) >= detail::as_tuple(rhs);
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