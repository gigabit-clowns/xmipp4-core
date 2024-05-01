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
 * @file layout_type.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Declares layout_type enum
 * @date 2024-05-01
 * 
 */

#include "../platform/constexpr.hpp"

#include <string_view>
#include <ostream>

namespace xmipp4 
{
namespace multidimensional
{

enum class layout_type
{
    unknown = -1,
    custom,
    column_major,
    row_major
};

XMIPP4_CONSTEXPR const char* to_string(layout_type axis) noexcept;
bool from_string(std::string_view str, layout_type& axis) noexcept;

template<typename T>
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, layout_type axis);





struct column_major_tag {};

/**
 * @brief Construct a column major tag
 * 
 * @return column_major_tag
 */
XMIPP4_CONSTEXPR column_major_tag column_major() noexcept;


struct row_major_tag {};

/**
 * @brief Construct a row major tag
 * 
 * @return row_major_tag
 */
XMIPP4_CONSTEXPR row_major_tag row_major() noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "layout_type.inl"
