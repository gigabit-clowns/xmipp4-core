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
 * @file layout_type.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of layout_type.hpp
 * @date 2024-05-01
 * 
 */

#include "layout_type.hpp"

#include <unordered_map>

namespace xmipp4 
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(layout_type axis) noexcept
{
    switch (axis)
    {
    case layout_type::unknown: return "unknown";
    case layout_type::custom: return "custom";
    case layout_type::column_major: return "column_major";
    case layout_type::row_major: return "row_major";
    default: return "";
    }
}

inline
bool from_string(std::string_view str, layout_type& axis) noexcept
{
    static const 
    std::unordered_map<std::string_view, layout_type> str_2_layout_type = 
    {
        { to_string(layout_type::unknown), layout_type::unknown },
        { to_string(layout_type::custom), layout_type::custom },
        { to_string(layout_type::column_major), layout_type::column_major },
        { to_string(layout_type::row_major), layout_type::row_major },
    };

    const auto ite = str_2_layout_type.find(str);
    const auto result = ite != str_2_layout_type.cend();
    if (result)
    {
        axis = ite->second;
    }

    return result;
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, layout_type axis)
{
    return os << to_string(axis);
}





XMIPP4_INLINE_CONSTEXPR column_major_tag column_major() noexcept
{
    return column_major_tag();
}

XMIPP4_INLINE_CONSTEXPR row_major_tag row_major() noexcept
{
    return row_major_tag();
}

} // namespace multidimensional
} // namespace xmipp4
