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

#include "platform/constexpr.hpp"

#include <string_view>

namespace xmipp4 
{

enum class axis_3d
{
    zero = 0,
    x = 1,
    y = 2,
    z = 3,
    negative_x = -x,
    negative_y = -y,
    negative_z = -z,
};

XMIPP4_CONSTEXPR axis_3d operator-(axis_3d axis) noexcept;

XMIPP4_CONSTEXPR bool is_negative(axis_3d axis) noexcept;
XMIPP4_CONSTEXPR bool is_positive(axis_3d axis) noexcept;
XMIPP4_CONSTEXPR axis_3d next_axis(axis_3d axis) noexcept;

XMIPP4_CONSTEXPR int dot(axis_3d left, axis_3d right) noexcept;
XMIPP4_CONSTEXPR axis_3d cross(axis_3d left, axis_3d right) noexcept;

XMIPP4_CONSTEXPR const char* to_string(axis_3d axis) noexcept;
bool from_string(std::string_view str, axis_3d& axis) noexcept;

} // namespace xmipp4

#include "axis_3d.inl"

