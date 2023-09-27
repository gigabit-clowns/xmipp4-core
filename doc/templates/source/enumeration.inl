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

#include "enumeration.hpp"

#include <unordered_map>

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(enumeration axis) noexcept
{
    switch (axis)
    {
    case enumeration::first: return "first";
    case enumeration::second: return "second";
    case enumeration::third: return "third";
    }
}

inline
bool from_string(std::string_view str, enumeration& axis) noexcept
{
    static const 
    std::unordered_map<std::string_view, enumeration> str_2_enumeration = 
    {
        { to_string(enumeration::first), enumeration::first },
        { to_string(enumeration::second), enumeration::second },
        { to_string(enumeration::third), enumeration::third },

    };

    const auto ite = str_2_enumeration.find(str);
    const auto result = ite != str_2_enumeration.cend();
    if (result)
    {
        axis = ite->second;
    }

    return result;
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, enumeration axis)
{
    return os << to_string(axis);
}

} // namespace xmipp4
