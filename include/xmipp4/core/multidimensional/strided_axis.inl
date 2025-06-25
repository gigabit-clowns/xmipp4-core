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
 * @file strided_axis.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of strided_axis
 * @date 2023-08-13
 * 
 */

#include "strided_axis.hpp"

#include "index.hpp"
#include "../math/abs.hpp"

namespace xmipp4
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR 
strided_axis::strided_axis(std::size_t extent, std::ptrdiff_t stride) noexcept
    : m_extent(extent)
    , m_stride(stride)
{   
}
    
XMIPP4_INLINE_CONSTEXPR 
bool strided_axis::operator==(const strided_axis& other) const noexcept
{
    return m_extent == other.m_extent && 
           m_stride == other.m_stride;
}

XMIPP4_INLINE_CONSTEXPR 
bool strided_axis::operator!=(const strided_axis& other) const noexcept
{
    return !(*this == other);
}

XMIPP4_INLINE_CONSTEXPR_CPP20
void strided_axis::swap(strided_axis &other) noexcept
{
    std::swap(m_extent, other.m_extent);
    std::swap(m_stride, other.m_stride);
}

XMIPP4_INLINE_CONSTEXPR 
void strided_axis::set_extent(std::size_t extent) noexcept
{
    m_extent = extent;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t strided_axis::get_extent() const noexcept
{
    return m_extent;
}

XMIPP4_INLINE_CONSTEXPR 
void strided_axis::set_stride(std::ptrdiff_t stride) noexcept
{
    m_stride = stride;
}

XMIPP4_INLINE_CONSTEXPR 
std::ptrdiff_t strided_axis::get_stride() const noexcept
{
    return m_stride;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t strided_axis::get_stride_magnitude() const noexcept
{
    return math::abs(get_stride());
}



XMIPP4_INLINE_CONSTEXPR_CPP20
void swap(strided_axis &x, strided_axis &y) noexcept
{
    x.swap(y);
}



XMIPP4_INLINE_CONSTEXPR
strided_axis make_contiguous_axis(std::size_t extent) noexcept
{
    return strided_axis(extent, 1);
}

XMIPP4_INLINE_CONSTEXPR
strided_axis make_phantom_axis(std::size_t extent) noexcept
{
    return strided_axis(extent, 0);
}


XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_equal(const strided_axis &lhs, 
                           const strided_axis &rhs ) noexcept
{
    return lhs.get_stride_magnitude() == rhs.get_stride_magnitude();
}

XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_less(const strided_axis &lhs, 
                          const strided_axis &rhs ) noexcept
{
    return lhs.get_stride_magnitude() < rhs.get_stride_magnitude();
}

XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_greater(const strided_axis &lhs, 
                             const strided_axis &rhs ) noexcept
{
    return lhs.get_stride_magnitude() > rhs.get_stride_magnitude();
}

XMIPP4_INLINE_CONSTEXPR
bool check_nonzero_stride(const strided_axis &axis) noexcept
{
    return axis.get_stride() != 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_contiguous(const strided_axis &axis) noexcept
{
    return axis.get_stride() == 1;
}

XMIPP4_INLINE_CONSTEXPR
bool is_contiguous(const strided_axis &major,
                   const strided_axis &minor ) noexcept
{
    const auto expected = major.get_stride()*major.get_extent();
    return expected == minor.get_stride();
}

XMIPP4_INLINE_CONSTEXPR
bool is_reversed(const strided_axis &axis) noexcept
{
    return axis.get_stride() < 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_repeating(const strided_axis &axis) noexcept
{
    return axis.get_extent() > 1 && axis.get_stride() == 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_empty(const strided_axis &axis) noexcept
{
    return axis.get_extent() == 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_significant(const strided_axis &axis) noexcept
{
    return axis.get_extent() != 1;
}

XMIPP4_INLINE_CONSTEXPR 
bool check_squeeze(const strided_axis &axis) noexcept
{
    return !is_significant(axis);
}

XMIPP4_INLINE_CONSTEXPR
bool get_axis_last_offset(const strided_axis &axis, 
                          std::ptrdiff_t &result) noexcept
{
    const auto extent = axis.get_extent();
    const bool success = (extent > 0);
    if (success)
    {
        const auto stride = axis.get_stride();
        result = (extent-1)*stride;
    }

    return success;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t get_axis_pivot_offset(const strided_axis &axis) noexcept
{
    const auto extent = axis.get_extent();
    const auto stride = axis.get_stride();
    return (stride < 0) && (extent > 1) ? (extent-1)*math::abs(stride) : 0;
}

XMIPP4_INLINE_CONSTEXPR
bool broadcast(strided_axis &axis1, strided_axis &axis2) noexcept
{
    bool result = true;

    if(axis1.get_extent() != axis2.get_extent())
    {
        if(axis1.get_extent() == 1)
            axis1 = make_phantom_axis(axis2.get_extent());
        else if(axis2.get_extent() == 1)
            axis2 = make_phantom_axis(axis1.get_extent());
        else
            result = false; // Unable to broadcast
    }

    return result;
}

XMIPP4_INLINE_CONSTEXPR
bool broadcast_to(strided_axis &from, std::size_t to) noexcept
{
    bool result = true;

    if(from.get_extent() != to)
    {
        if(from.get_extent() == 1)
            from = make_phantom_axis(to);
        else
            result = false; // Unable to broadcast
    }

    return result;
}

} // namespace multidimensional
} // namespace xmipp4
