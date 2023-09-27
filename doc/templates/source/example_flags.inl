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

#include "example_flags.hpp"

#include <unordered_map>

namespace xmipp4
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(example_flag_bits ex) noexcept
{
    switch (ex)
    {
    case example_flag_bits::first: return "first";
    case example_flag_bits::second: return "second";
    case example_flag_bits::third: return "third";
    }
}

inline
bool from_string(std::string_view str, example_flag_bits& ex) noexcept
{
    static const
    std::unordered_map<std::string_view, example_flag_bits> str_2_example_flag_bits = 
    {
        { to_string(example_flag_bits::first), example_flag_bits::first },
        { to_string(example_flag_bits::second), example_flag_bits::second },
        { to_string(example_flag_bits::third), example_flag_bits::third },
    };

    const auto ite = str_2_example_flag_bits.find(str);
    const bool result = ite != str_2_example_flag_bits.end();
    if(result)
        ex = ite->second;
    return result;
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, enumeration ex)
{
    return os << to_string(ex);
}

} // namespace xmipp4
