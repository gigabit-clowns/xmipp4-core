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
 * @file axis_descriptor.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of axis_descriptor
 * @date 2023-08-13
 * 
 */

#include "axis_descriptor.hpp"

#include "index.hpp"
#include "../math/abs.hpp"

namespace xmipp4
{
namespace multidimensional
{

XMIPP4_INLINE_CONSTEXPR axis_descriptor::axis_descriptor(std::size_t extent, 
                                                         std::ptrdiff_t stride ) noexcept
    : m_extent(extent)
    , m_stride(stride)
{   
}
    
XMIPP4_INLINE_CONSTEXPR 
bool axis_descriptor::operator==(const axis_descriptor& other) const noexcept
{
    return m_extent == other.m_extent && 
           m_stride == other.m_stride;
}

XMIPP4_INLINE_CONSTEXPR 
bool axis_descriptor::operator!=(const axis_descriptor& other) const noexcept
{
    return !(*this == other);
}

XMIPP4_INLINE_CONSTEXPR_CPP20
void axis_descriptor::swap(axis_descriptor &other) noexcept
{
    std::swap(m_extent, other.m_extent);
    std::swap(m_stride, other.m_stride);
}

XMIPP4_INLINE_CONSTEXPR 
void axis_descriptor::set_extent(std::size_t extent) noexcept
{
    m_extent = extent;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t axis_descriptor::get_extent() const noexcept
{
    return m_extent;
}

XMIPP4_INLINE_CONSTEXPR 
void axis_descriptor::set_stride(std::ptrdiff_t stride) noexcept
{
    m_stride = stride;
}

XMIPP4_INLINE_CONSTEXPR 
std::ptrdiff_t axis_descriptor::get_stride() const noexcept
{
    return m_stride;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t axis_descriptor::get_stride_magnitude() const noexcept
{
    return math::abs(get_stride());
}



XMIPP4_INLINE_CONSTEXPR_CPP20
void swap(axis_descriptor &x, axis_descriptor &y) noexcept
{
    x.swap(y);
}



XMIPP4_INLINE_CONSTEXPR
axis_descriptor make_contiguous_axis(std::size_t extent) noexcept
{
    return axis_descriptor(extent, 1);
}

XMIPP4_INLINE_CONSTEXPR
axis_descriptor make_phantom_axis(std::size_t extent) noexcept
{
    return axis_descriptor(extent, 0);
}


XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_equal(const axis_descriptor &lhs, 
                           const axis_descriptor &rhs ) noexcept
{
    return lhs.get_stride_magnitude() == rhs.get_stride_magnitude();
}

XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_less(const axis_descriptor &lhs, 
                          const axis_descriptor &rhs ) noexcept
{
    return lhs.get_stride_magnitude() < rhs.get_stride_magnitude();
}

XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_greater(const axis_descriptor &lhs, 
                             const axis_descriptor &rhs ) noexcept
{
    return lhs.get_stride_magnitude() > rhs.get_stride_magnitude();
}

XMIPP4_INLINE_CONSTEXPR
bool check_nonzero_stride(const axis_descriptor &axis) noexcept
{
    return axis.get_stride() != 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_contiguous(const axis_descriptor &axis) noexcept
{
    return axis.get_stride() == 1;
}

XMIPP4_INLINE_CONSTEXPR
bool is_contiguous(const axis_descriptor &major,
                   const axis_descriptor &minor ) noexcept
{
    const auto expected = major.get_stride()*major.get_extent();
    return expected == minor.get_stride();
}

XMIPP4_INLINE_CONSTEXPR
bool is_mirror_contiguous(const axis_descriptor &axis) noexcept
{
    return axis.get_stride_magnitude() == 1;
}

XMIPP4_INLINE_CONSTEXPR
bool is_mirror_contiguous(const axis_descriptor &major,
                   const axis_descriptor &minor ) noexcept
{
    const auto expected = major.get_stride_magnitude()*major.get_extent();
    return expected == minor.get_stride_magnitude();
}

XMIPP4_INLINE_CONSTEXPR
bool is_reversed(const axis_descriptor &axis) noexcept
{
    return axis.get_stride() < 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_repeating(const axis_descriptor &axis) noexcept
{
    return axis.get_extent() > 1 && axis.get_stride() == 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_empty(const axis_descriptor &axis) noexcept
{
    return axis.get_extent() == 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_significant(const axis_descriptor &axis) noexcept
{
    return axis.get_extent() != 1;
}

XMIPP4_INLINE_CONSTEXPR 
bool check_squeeze(const axis_descriptor &axis) noexcept
{
    return !is_significant(axis);
}

XMIPP4_INLINE_CONSTEXPR
bool get_axis_last_offset(const axis_descriptor &axis, 
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
std::size_t get_axis_pivot_offset(const axis_descriptor &axis) noexcept
{
    const auto extent = axis.get_extent();
    const auto stride = axis.get_stride();
    return (stride < 0) && (extent > 1) ? (extent-1)*math::abs(stride) : 0;
}

XMIPP4_INLINE_CONSTEXPR
bool broadcast(axis_descriptor &axis, std::size_t &extent) noexcept
{
    bool result = true;

    if(axis.get_extent() != extent)
    {
        if(axis.get_extent() == 1)
            axis = make_phantom_axis(extent);
        else if(extent == 1)
            extent = axis.get_extent();
        else
            result = false; // Unable to broadcast
    }

    return result;
}

XMIPP4_INLINE_CONSTEXPR
bool broadcast_to(axis_descriptor &axis, std::size_t extent) noexcept
{
    bool result = true;

    if(axis.get_extent() != extent)
    {
        if(axis.get_extent() == 1)
            axis = make_phantom_axis(extent);
        else
            result = false; // Unable to broadcast
    }

    return result;
}



template <typename I>
inline
void apply_index(const axis_descriptor &desc,
                 I index,
                 std::ptrdiff_t &offset )
{
    offset += sanitize_index(index, desc.get_extent()) * desc.get_stride();
}

template <typename Start, typename Stop, typename Step>
inline
void apply_slice(axis_descriptor &desc, 
                 const slice<Start, Stop, Step> &slc,
                 std::ptrdiff_t &offset )
{
    std::size_t start;
    std::size_t stop;
    std::ptrdiff_t step;
    sanitize_slice(
        slc, desc.get_extent(),
        start, stop, step
    );
    const auto pivot = compute_slice_pivot(start, step);
    const auto extent = compute_slice_size(start, stop, step);
    
    auto stride = desc.get_stride();
    offset += stride*pivot;
    stride *= step;
    desc = axis_descriptor(extent, stride);
}

} // namespace multidimensional
} // namespace xmipp4
