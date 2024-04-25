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
#include "../index.hpp"

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
std::size_t axis_descriptor::get_unsigned_stride() const noexcept
{
    //FIXME Using a manual abs because std::abs is not constexpr. Use a proper abs function
    const auto stride = get_stride();
    return static_cast<std::size_t>(stride < 0 ? -stride : stride);
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t axis_descriptor::get_width() const noexcept
{
    return get_unsigned_stride()*get_extent();
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



XMIPP4_INLINE_CONSTEXPR_CPP20
void swap(axis_descriptor &x, axis_descriptor &y) noexcept
{
    x.swap(y);
}



template <typename I>
inline
void apply_index(const axis_descriptor &desc,
                 I index,
                 std::ptrdiff_t &offset )
{
    offset += sanitize_index(index, desc.get_extent()) * desc.get_stride();
}

template <typename Start, typename Stride, typename Stop>
inline
axis_descriptor apply_slice(const axis_descriptor &desc, 
                            const slice<Start, Stride, Stop> &slc,
                            std::ptrdiff_t &offset )
{
    std::size_t start;
    std::size_t stop;
    std::ptrdiff_t stride;
    sanitize_slice(
        slc, desc.get_extent(),
        start, stop, stride
    );
    const auto pivot = compute_slice_pivot(start, stride);
    const auto extent = compute_slice_size(start, stop, stride);
    
    offset += desc.get_stride()*pivot;
    stride *= desc.get_stride();
    return axis_descriptor(extent, stride);
}

} // namespace multidimensional
} // namespace xmipp4
