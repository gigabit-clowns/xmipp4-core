// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file axis_3d.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of axis_3d.hpp
 * @date 2023-09-27
 * 
 */

#include "axis_3d.hpp"

#include "platform/assert.hpp"

#include <unordered_map>
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

XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR 
axis_3d next_axis(axis_3d axis) noexcept
{
    switch (axis)
    {
    case axis_3d::x: return axis_3d::y;
    case axis_3d::y: return axis_3d::z;
    case axis_3d::z: return axis_3d::x;
    case axis_3d::negative_x: return axis_3d::negative_z;
    case axis_3d::negative_y: return axis_3d::negative_x;
    case axis_3d::negative_z: return axis_3d::negative_y;
    default: return axis_3d::zero; // case axis_3d::zero
    }
}

XMIPP4_INLINE_CONSTEXPR 
int dot(axis_3d left, axis_3d right) noexcept
{
    int result = 0;
    if (left != axis_3d::zero && right != axis_3d::zero)
    {
        if (left == right)
            result = 1;
        else if (left == -right)
            result = -1;
    }

    return result;
}

XMIPP4_INLINE_CONSTEXPR 
axis_3d cross(axis_3d left, axis_3d right) noexcept
{
    auto result = axis_3d::zero;
    if (left != axis_3d::zero && right != axis_3d::zero)
    {
        // Assure that both axes are positive
        const auto left_is_negative = is_negative(left);
        if (left_is_negative) left = -left;
        const auto right_is_negative = is_negative(right);
        if (right_is_negative) right = -right;

        if (left != right)
        {
            // Determine the sign of the result
            bool result_is_negative = left_is_negative != right_is_negative; //xor

            // Assure that the axes are ordered (x->y->z->x->...)
            if(next_axis(left) != right)
            {
                result_is_negative = !result_is_negative; // Negate the result when reordering
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
            else // left == axis_3d::z
            {
                XMIPP4_ASSERT(left == axis_3d::z);
                XMIPP4_ASSERT(right == axis_3d::x);
                result = axis_3d::y;
            }

            // Copy the sign
            if (result_is_negative)
                result = -result;
        }
    }

    return result;
}

XMIPP4_INLINE_CONSTEXPR 
const char* to_string(axis_3d axis) noexcept
{
    switch (axis)
    {
    case axis_3d::zero: return "0";
    case axis_3d::x: return "x";
    case axis_3d::y: return "y";
    case axis_3d::z: return "z";
    case axis_3d::negative_x: return "-x";
    case axis_3d::negative_y: return "-y";
    case axis_3d::negative_z: return "-z";
    default: return "";
    }
}

inline
bool from_string(std::string_view str, axis_3d& axis) noexcept
{
    static const 
    std::unordered_map<std::string_view, axis_3d> str_2_axis_3d = 
    {
        { to_string(axis_3d::zero), axis_3d::zero },
        { "zero", axis_3d::zero },
        { to_string(axis_3d::x), axis_3d::x },
        { "+x", axis_3d::x },
        { to_string(axis_3d::y), axis_3d::y },
        { "+y", axis_3d::y },
        { to_string(axis_3d::z), axis_3d::z },
        { "+z", axis_3d::z },
        { to_string(axis_3d::negative_x), axis_3d::negative_x },
        { to_string(axis_3d::negative_y), axis_3d::negative_y },
        { to_string(axis_3d::negative_z), axis_3d::negative_z },

    };

    const auto ite = str_2_axis_3d.find(str);
    const auto result = ite != str_2_axis_3d.cend();
    if (result)
    {
        axis = ite->second;
    }

    return result;
}

template<typename T>
inline
std::basic_ostream<T>& operator<<(std::basic_ostream<T>& os, axis_3d axis)
{
    return os << to_string(axis);
}

} // namespace xmipp4
