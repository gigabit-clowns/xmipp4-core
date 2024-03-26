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
 * @file slice.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of slice.hpp
 * @date 2023-08-13
 * 
 */

#include "slice.hpp"

namespace xmipp4 
{

namespace detail
{

// Trivial case: No type change
template <typename To, typename From>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<To>::value && std::is_same<From, To>::value, To>::type
propagate_end(From x)
{
    return x;
}

// Casting to a different integer type. Propagate end values carefully
template <typename To, typename From>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<To>::value && std::is_integral<From>::value, To>::type
propagate_end(From x)
{
    const auto is_end = (x == static_cast<From>(end));
    return is_end ? static_cast<To>(end) : static_cast<To>(x);
}

// Casting an end tag to integer type. Force an end value
template <typename To>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<To>::value, To>::type
propagate_end(end_tag)
{
    return static_cast<To>(end); 
}

// Integral constant with end value
template <typename To, typename From>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<To>::value, To>::type
propagate_end(std::integral_constant<From, static_cast<From>(end)> v)
{
    return static_cast<To>(end);
}

// General case with integral_constant (do not propagate)
template <typename To, typename From, From value>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<To>::value, To>::type
propagate_end(std::integral_constant<From, value> v)
{
    return static_cast<To>(v);
}

}

template <typename Start, typename Stride, typename Stop>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Stride, Stop>::slice(start_type start, 
                                  stride_type stride, 
                                  stop_type stop ) noexcept
    : m_start(start)
    , m_stride(stride)
    , m_stop(stop)
{
}

template <typename Start, typename Stride, typename Stop>
template <typename Start2, typename Stride2, typename Stop2>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Stride, Stop>::slice(const slice<Start2, Stride2, Stop2>& other) noexcept
    : slice(
        other.get_start(),
        static_cast<stride_type>(other.get_stride()),
        detail::propagate_end<stop_type>(other.get_stop())
    )
{
}

template <typename Start, typename Stride, typename Stop>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Stride, Stop>::set_start(start_type start) noexcept
{
    m_start = start;
}

template <typename Start, typename Stride, typename Stop>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Stride, Stop>::start_type& 
slice<Start, Stride, Stop>::get_start() const noexcept
{
    return m_start;
}

template <typename Start, typename Stride, typename Stop>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Stride, Stop>::set_stride(stride_type stride) noexcept
{
    m_stride = stride;
}

template <typename Start, typename Stride, typename Stop>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Stride, Stop>::stride_type&
slice<Start, Stride, Stop>::get_stride() const noexcept
{
    return m_stride;
}

template <typename Start, typename Stride, typename Stop>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Stride, Stop>::set_stop(stop_type stop) noexcept
{
    m_stop = stop;
}

template <typename Start, typename Stride, typename Stop>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Stride, Stop>::stop_type& 
slice<Start, Stride, Stop>::get_stop() const noexcept
{
    return m_stop;
}



template <typename Start1, typename Stride1, typename Stop1, 
          typename Start2, typename Stride2, typename Stop2>
XMIPP4_INLINE_CONSTEXPR bool
operator==( const slice<Start1, Stride1, Stop1>& lhs, 
            const slice<Start2, Stride2, Stop2>& rhs ) noexcept
{
    return lhs.get_start() == rhs.get_start() &&
           lhs.get_stride() == rhs.get_stride() &&
           lhs.get_stop() == rhs.get_stop() ;
}

template <typename Start1, typename Stride1, typename Stop1, 
          typename Start2, typename Stride2, typename Stop2>
XMIPP4_INLINE_CONSTEXPR bool
operator!=( const slice<Start1, Stride1, Stop1>& lhs, 
            const slice<Start2, Stride2, Stop2>& rhs ) noexcept
{
    return !(lhs == rhs);
}

template <typename Start, typename Stride, typename Stop>
inline std::ostream& 
operator<<(std::ostream& os, const slice<Start, Stride, Stop> &s)
{
    return os << "slice(" << s.get_start()
              << ", " << s.get_stride()
              << ", " << s.get_stop() << ")";
}





XMIPP4_INLINE_CONSTEXPR bool
operator==(const begin_tag&, const begin_tag&) noexcept
{
    return true;
}

XMIPP4_INLINE_CONSTEXPR bool
operator!=(const begin_tag&, const begin_tag&) noexcept
{
    return false;
}

inline std::ostream& operator<<(std::ostream& os, begin_tag)
{
    return os << "begin";
}



XMIPP4_CONSTEXPR bool
operator==(const adjacent_tag&, const adjacent_tag&) noexcept
{
    return true;
}

XMIPP4_CONSTEXPR bool
operator!=(const adjacent_tag&, const adjacent_tag&) noexcept
{
    return false;
}

inline std::ostream& operator<<(std::ostream& os, adjacent_tag)
{
    return os << "adjacent";
}



XMIPP4_CONSTEXPR bool
operator==(const end_tag& lhs, const end_tag& rhs) noexcept
{
    return true;
}

XMIPP4_CONSTEXPR bool
operator!=(const end_tag& lhs, const end_tag& rhs) noexcept
{
    return true;
}

inline std::ostream& operator<<(std::ostream& os, end_tag)
{
    return os << "end";
}

inline std::ostream& operator<<(std::ostream& os, all_tag)
{
    return os << "all";
}


template <typename Stop>
XMIPP4_INLINE_CONSTEXPR slice<begin_tag, adjacent_tag, Stop> 
make_slice(Stop stop) noexcept
{
    return slice<begin_tag, adjacent_tag, Stop>(
        begin, 
        adjacent,  
        stop
    );
}

template <typename Start, typename Stop>
XMIPP4_INLINE_CONSTEXPR slice<Start, adjacent_tag, Stop> 
make_slice(Start start, Stop stop) noexcept
{
    return slice<Start, adjacent_tag, Stop>(
        start, 
        adjacent, 
        stop
    );
}

template <typename Start, typename Stride, typename Stop>
XMIPP4_INLINE_CONSTEXPR slice<Start, Stride, Stop> 
make_slice(Start start, Stride stride, Stop stop) noexcept
{
    return slice<Start, Stride, Stop>(
        start, 
        stride, 
        stop
    );
}

} // namespace xmipp4
