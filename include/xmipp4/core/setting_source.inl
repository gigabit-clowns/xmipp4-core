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
 * @file setting_source.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of setting_source.hpp
 * @date 2025-05-09
 * 
 */

#include "setting_source.hpp"

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR 
bool operator<(setting_source lhs, setting_source rhs) noexcept
{
    return static_cast<std::underlying_type<setting_source>::type>(lhs) <
           static_cast<std::underlying_type<setting_source>::type>(rhs) ;  
}

XMIPP4_INLINE_CONSTEXPR 
bool operator<=(setting_source lhs, setting_source rhs) noexcept
{
    return static_cast<std::underlying_type<setting_source>::type>(lhs) <=
           static_cast<std::underlying_type<setting_source>::type>(rhs) ;  
}

XMIPP4_INLINE_CONSTEXPR 
bool operator>(setting_source lhs, setting_source rhs) noexcept
{
    return rhs < lhs;
}

XMIPP4_INLINE_CONSTEXPR 
bool operator>=(setting_source lhs, setting_source rhs) noexcept
{
    return rhs <= lhs;
}



XMIPP4_INLINE_CONSTEXPR 
const char* to_string(setting_source priority) noexcept
{
    switch (priority)
    {
    case setting_source::unset: return "unset";
    case setting_source::preset: return "preset";
    case setting_source::environment_variable: return "environment_variable";
    case setting_source::configuration_file: return "configuration_file";
    case setting_source::user: return "user";
    default: return "";
    }
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, 
                                  setting_source priority )
{
    return os << to_string(priority);
}

} // namespace xmipp4
