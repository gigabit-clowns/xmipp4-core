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
 * @file backend_priority.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of backend_priority.hpp
 * @date 2024-11-20
 * 
 */

#include "backend_priority.hpp"

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR 
bool operator<(backend_priority lhs, backend_priority rhs) noexcept
{
    return static_cast<std::underlying_type<backend_priority>::type>(lhs) <
           static_cast<std::underlying_type<backend_priority>::type>(rhs) ;  
}

XMIPP4_INLINE_CONSTEXPR 
bool operator<=(backend_priority lhs, backend_priority rhs) noexcept
{
    return static_cast<std::underlying_type<backend_priority>::type>(lhs) <=
           static_cast<std::underlying_type<backend_priority>::type>(rhs) ;  
}

XMIPP4_INLINE_CONSTEXPR 
bool operator>(backend_priority lhs, backend_priority rhs) noexcept
{
    return rhs < lhs;
}

XMIPP4_INLINE_CONSTEXPR 
bool operator>=(backend_priority lhs, backend_priority rhs) noexcept
{
    return rhs <= lhs;
}



XMIPP4_INLINE_CONSTEXPR 
const char* to_string(backend_priority priority) noexcept
{
    switch (priority)
    {
    case backend_priority::fallback: return "fallback";
    case backend_priority::normal: return "normal";
    case backend_priority::preferred: return "preferred";
    default: return "";
    }
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, 
                                  backend_priority priority )
{
    return os << to_string(priority);
}

} // namespace xmipp4
