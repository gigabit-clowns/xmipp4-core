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
 * @file layout_flags.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of layout_flags.hpp
 * @date 2024-05-01
 * 
 */

#include "layout_flags.hpp"

#include <unordered_map>

namespace xmipp4 
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(layout_flag_bits flag) noexcept
{
    switch (flag)
    {
    case layout_flag_bits::contiguous: return "contiguous";
    case layout_flag_bits::column_major: return "column_major";
    case layout_flag_bits::row_major: return "row_major";
    default: return "";
    }
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, layout_flag_bits flag)
{
    return os << to_string(flag);
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
