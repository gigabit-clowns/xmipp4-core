// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file axis_descriptor.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of axis_descriptor
 * @date 2023-08-13
 * 
 */

#include "axis_descriptor.hpp"

#include "../index.hpp"
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
std::size_t axis_descriptor::get_unsigned_stride() const noexcept
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
std::ptrdiff_t get_axis_last_position(const axis_descriptor &axis) noexcept
{
    std::ptrdiff_t result = 0UL;
    const auto extent = axis.get_extent();
    if (extent > 0)
    {   
        const auto stride = axis.get_unsigned_stride();
        result = (extent-1)*stride;
    }
    else
    {
        result = -1;
    }

    return result;
}

XMIPP4_INLINE_CONSTEXPR 
std::size_t get_axis_length(const axis_descriptor &axis) noexcept
{
    return axis.get_unsigned_stride()*axis.get_extent();
}

XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_equal(const axis_descriptor &lhs, 
                           const axis_descriptor &rhs ) noexcept
{
    return lhs.get_unsigned_stride() == rhs.get_unsigned_stride();
}

XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_less(const axis_descriptor &lhs, 
                          const axis_descriptor &rhs ) noexcept
{
    return lhs.get_unsigned_stride() < rhs.get_unsigned_stride();
}

XMIPP4_INLINE_CONSTEXPR 
bool compare_strides_greater(const axis_descriptor &lhs, 
                             const axis_descriptor &rhs ) noexcept
{
    return lhs.get_unsigned_stride() > rhs.get_unsigned_stride();
}

XMIPP4_INLINE_CONSTEXPR
bool check_nonzero_stride(const axis_descriptor &axis) noexcept
{
    return axis.get_stride() != 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_contiguous(const axis_descriptor &axis) noexcept
{
    return axis.get_unsigned_stride() == 1;
}

XMIPP4_INLINE_CONSTEXPR
bool is_regular(const axis_descriptor &major,
                const axis_descriptor &minor ) noexcept
{
    const auto expected = major.get_unsigned_stride()*major.get_extent();
    return expected == minor.get_unsigned_stride();
}

XMIPP4_INLINE_CONSTEXPR
bool is_reversed(const axis_descriptor &axis) noexcept
{
    return axis.get_stride() < 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_significant(const axis_descriptor &axis) noexcept
{
    return axis.get_extent() != 1 && axis.get_stride() != 0;
}

XMIPP4_INLINE_CONSTEXPR
bool is_repeating(const axis_descriptor &axis) noexcept
{
    return axis.get_extent() > 1 && axis.get_stride() == 0;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t get_reverse_axis_offset(const axis_descriptor &axis) noexcept
{
    const auto extent = axis.get_extent();
    const auto stride = axis.get_stride();
    return (stride < 0) && (extent > 1) ? (extent-1)*math::abs(stride) : 0;
}

XMIPP4_INLINE_CONSTEXPR 
bool check_squeeze(const axis_descriptor &axis) noexcept
{
    return axis.get_extent() == 1;
}

namespace detail
{

// No axis to broadcast. Always succeed.
XMIPP4_INLINE_CONSTEXPR
bool broadcast() noexcept
{
    return true;
}

// Single axis to broadcast. Always succeed.
XMIPP4_INLINE_CONSTEXPR
bool broadcast(const axis_descriptor&) noexcept
{
    return true;
}

/// Broadcast a pair of axes.
XMIPP4_INLINE_CONSTEXPR
bool broadcast(axis_descriptor &x, axis_descriptor &y) noexcept
{
    bool result = true;

    if(x.get_extent() != y.get_extent())
    {
        if(x.get_extent() == 1)
            x = make_phantom_axis(y.get_extent());
        else if(y.get_extent() == 1)
            y = make_phantom_axis(x.get_extent());
        else
            result = false; // Unable to broadcast
    }

    return result;
}

/// Broadcast N>2 axes.
template<typename... AxisDescriptors>
XMIPP4_INLINE_CONSTEXPR
bool broadcast(axis_descriptor& first, 
               axis_descriptor& second,
               axis_descriptor& third,
               AxisDescriptors&... others ) noexcept
{
    // This code recursively explores adjacent items in a ping-pong
    // pattern, so that changes propagate back to the first element.
    // For instance, when called with 4 arguments, it leads to the 
    // comparing broadcast for the following combinations:
    // (0, 1) (1, 2) (2, 3) (1, 2) (0, 1)

    if(!broadcast(first, second)) // Forward propagate
        return false;
    if(!broadcast(second, third, others...)) // Recurse
        return false;
    if(!broadcast(first, second)) // Back propagate 
        return false;

    return true;
}

} // namespace detail

template<typename... AxisDescriptors>
XMIPP4_INLINE_CONSTEXPR
bool broadcast(AxisDescriptors&... axes) noexcept
{
    return detail::broadcast(axes...);
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
axis_descriptor apply_slice(const axis_descriptor &desc, 
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
    return axis_descriptor(extent, stride);
}

} // namespace multidimensional
} // namespace xmipp4
