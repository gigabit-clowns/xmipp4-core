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

#include "axis_3d.hpp"

#include "platform/assert.hpp"

#include <type_traits>

namespace xmipp4 
{

XMIPP4_INLINE_CONSTEXPR 
axis_3d operator-(axis_3d axis) noexcept
{
    return static_cast<axis_3d>(-static_cast<std::underlying_type<axis_3d>::type>(axis));
}

XMIPP4_INLINE_CONSTEXPR 
bool is_negative(axis_3d axis) noexcept
{
    return static_cast<std::underlying_type<axis_3d>::type>(axis) < 0;
}

XMIPP4_INLINE_CONSTEXPR 
bool is_positive(axis_3d axis) noexcept
{
    return static_cast<std::underlying_type<axis_3d>::type>(axis) > 0;
}

XMIPP4_INLINE_CONSTEXPR 
axis_3d next_axis(axis_3d axis) noexcept
{
    switch (axis)
    {
        case axis_3d::zero: return axis_3d::zero;
        case axis_3d::x: return axis_3d::y;
        case axis_3d::y: return axis_3d::z;
        case axis_3d::z: return axis_3d::x;
        case axis_3d::minus_x: return axis_3d::minus_z;
        case axis_3d::minus_y: return axis_3d::minus_x;
        case axis_3d::minus_z: return axis_3d::minus_y;
    }
}

XMIPP4_INLINE_CONSTEXPR 
int dot(axis_3d left, axis_3d right) noexcept
{
    if (left == right)
        return 1;
    else if (left == -right)
        return -1;
    else
        return 0;
}

XMIPP4_INLINE_CONSTEXPR 
axis_3d cross(axis_3d left, axis_3d right) noexcept
{
    auto result = axis_3d::zero;

    if (left != axis_3d::zero && right != axis_3d::zero)
    {
        // Assure that both axes are positive (or zero)
        const auto left_is_negative = is_negative(left);
        if (left_is_negative) left = -left;
        const auto right_is_negative = is_negative(right);
        if (right_is_negative) right = -right;

        // Determine the sign of the result
        bool result_is_negative = left_is_negative != right_is_negative;

        // Assure that the axes are ordered (x->y->z->x->...)
        if(next_axis(left) != right)
        {
            // Flip te sign of the result
            result_is_negative = !result_is_negative;
            std::swap(left, right);
        }

        // Manual implementation of ordered positive axis cross products.
        // There are only 3 possible combinations
        if(left == axis_3d::x)
        {
            XMIPP4_ASSERT(right == axis_3d::y);
            result = axis_3d::z;
        }
        else if(left == axis_3d::y)
        {
            XMIPP4_ASSERT(right == axis_3d::z);
            result = axis_3d::x;
        }
        else
        {
            XMIPP4_ASSERT(left == axis_3d::z);
            XMIPP4_ASSERT(right == axis_3d::x);
            result = axis_3d::y;
        }

        // Copy the sign
        if (result_is_negative)
            result = -result;
    }

    return result;
}

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(axis_3d type) noexcept;

inline
axis_3d from_string(std::string_view str);

} // namespace xmipp4