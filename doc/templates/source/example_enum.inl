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

#include "example_enum.hpp"

#include <unordered_map>

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(example_enum ex) noexcept
{
    switch (ex)
    {
    case example_enum::first: return "first";
    case example_enum::second: return "second";
    case example_enum::third: return "third";
    }
}

inline
bool from_string(std::string_view str, example_enum& ex) noexcept
{
    static const 
    std::unordered_map<std::string_view, example_enum> str_2_example_enum = 
    {
        { to_string(example_enum::first), example_enum::first },
        { to_string(example_enum::second), example_enum::second },
        { to_string(example_enum::third), example_enum::third },

    };

    const auto ite = str_2_example_enum.find(str);
    const auto result = ite != str_2_example_enum.cend();
    if (result)
    {
        ex = ite->second;
    }

    return result;
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, example_enum ex)
{
    return os << to_string(ex);
}

} // namespace xmipp4
