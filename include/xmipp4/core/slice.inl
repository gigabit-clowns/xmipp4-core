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

#include <sstream>

namespace xmipp4 
{

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
slice<Start, Stride, Stop>::slice(Start2 start,
                                  Stride2 stride,
                                  Stop2 stop ) noexcept
    : slice(
        static_cast<start_type>(start),
        static_cast<stride_type>(stride),
        propagate_end<stop_type>(stop)
    )
{
}

template <typename Start, typename Stride, typename Stop>
template <typename Start2, typename Stride2, typename Stop2>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Stride, Stop>::slice(const slice<Start2, Stride2, Stop2>& other) noexcept
    : slice(other.get_start(), other.get_stride(), other.get_stop())
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





XMIPP4_INLINE_CONSTEXPR begin_tag begin() noexcept
{
    return begin_tag();
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

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(const begin_tag& lhs, I rhs) noexcept
{
    return static_cast<I>(lhs) == rhs;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(const begin_tag& lhs, I rhs) noexcept
{
    return static_cast<I>(lhs) != rhs;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(I lhs, const begin_tag& rhs) noexcept
{
    return lhs == static_cast<I>(rhs);
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(I lhs, const begin_tag& rhs) noexcept
{
    return lhs != static_cast<I>(rhs);
}

inline std::ostream& operator<<(std::ostream& os, begin_tag)
{
    return os << "begin";
}



XMIPP4_INLINE_CONSTEXPR adjacent_tag adjacent() noexcept
{
    return adjacent_tag();
}

XMIPP4_INLINE_CONSTEXPR bool
operator==(const adjacent_tag&, const adjacent_tag&) noexcept
{
    return true;
}

XMIPP4_INLINE_CONSTEXPR bool
operator!=(const adjacent_tag&, const adjacent_tag&) noexcept
{
    return false;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(const adjacent_tag& lhs, I rhs) noexcept
{
    return static_cast<I>(lhs) == rhs;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(const adjacent_tag& lhs, I rhs) noexcept
{
    return static_cast<I>(lhs) != rhs;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(I lhs, const adjacent_tag& rhs) noexcept
{
    return lhs == static_cast<I>(rhs);
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(I lhs, const adjacent_tag& rhs) noexcept
{
    return lhs != static_cast<I>(rhs);
}

inline std::ostream& operator<<(std::ostream& os, adjacent_tag)
{
    return os << "adjacent";
}



namespace detail
{

template <typename To, typename From>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_same<From, To>::value, To>::type
propagate_end(From&& x)
{
    return std::forward<From>(x);
}

template <typename To, typename From>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<To>::value && 
                        std::is_integral<From>::value && 
                        !std::is_same<From, To>::value, To>::type
propagate_end(From x)
{
    const auto is_end = x == end();
    return is_end ? static_cast<To>(end()) : static_cast<To>(x);
}

template <typename To>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<To>::value, To>::type
propagate_end(end_tag)
{
    return end(); 
}

template <typename To, typename From, From value>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_integral<To>::value, To>::type
propagate_end(std::integral_constant<From, value>)
{
    return propagate_end<To>(value);
}

} // namespace detail

XMIPP4_INLINE_CONSTEXPR end_tag end() noexcept
{
    return end_tag();
}

template <typename To, typename From>
XMIPP4_NODISCARD XMIPP4_INLINE_CONSTEXPR
To propagate_end(From x) noexcept
{
    return detail::propagate_end<To>(x);
}

XMIPP4_INLINE_CONSTEXPR bool
operator==(const end_tag&, const end_tag&) noexcept
{
    return true;
}

XMIPP4_INLINE_CONSTEXPR bool
operator!=(const end_tag&, const end_tag&) noexcept
{
    return true;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(const end_tag& lhs, I rhs) noexcept
{
    return static_cast<I>(lhs) == rhs;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(const end_tag& lhs, I rhs) noexcept
{
    return static_cast<I>(lhs) != rhs;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(I lhs, const end_tag& rhs) noexcept
{
    return lhs == static_cast<I>(rhs);
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(I lhs, const end_tag& rhs) noexcept
{
    return lhs != static_cast<I>(rhs);
}

inline std::ostream& operator<<(std::ostream& os, end_tag)
{
    return os << "end";
}



XMIPP4_INLINE_CONSTEXPR all_tag all() noexcept
{
    return all_tag();
}

inline std::ostream& operator<<(std::ostream& os, all_tag)
{
    return os << "all";
}

XMIPP4_INLINE_CONSTEXPR even_tag even() noexcept
{
    return even_tag();
}

XMIPP4_INLINE_CONSTEXPR odd_tag odd() noexcept
{
    return odd_tag();
}




template <typename Stop>
XMIPP4_INLINE_CONSTEXPR slice<begin_tag, adjacent_tag, Stop> 
make_slice(Stop stop) noexcept
{
    return slice<begin_tag, adjacent_tag, Stop>(
        begin(), 
        adjacent(),  
        stop
    );
}

template <typename Start, typename Stop>
XMIPP4_INLINE_CONSTEXPR slice<Start, adjacent_tag, Stop> 
make_slice(Start start, Stop stop) noexcept
{
    return slice<Start, adjacent_tag, Stop>(
        start, 
        adjacent(), 
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





namespace detail
{

template <typename I>
inline
typename std::enable_if<std::is_integral<I>::value && std::is_signed<I>::value, std::size_t>::type
sanitize_slice_index(I index, std::size_t size)
{
    std::size_t result;

    if (index == end())
    {
        result = size;
    }
    else
    {
        if (index < 0)
        {  
            if (index < (-size))
            {
                std::ostringstream oss;
                oss << "Index (" << index << ") must be greater or equal than "
                    << "the negative array size (" << (-size) << ").";

                throw std::out_of_range(oss.str());
            }

            result = index + size;
        }
        else
        {
            if(index > size)
            {
                std::ostringstream oss;
                oss << "Index (" << index << ") must be less or equal than "
                    << "the axis size (" << size << ").";

                throw std::out_of_range(oss.str());
            }

            result = index;
        }
    }

    return result;
}

template <typename I>
inline
typename std::enable_if<std::is_integral<I>::value && !std::is_signed<I>::value, std::size_t>::type
sanitize_slice_index(I index, std::size_t size)
{
    std::size_t result;

    if (index == end())
    {
        result = size;
    }
    else
    {
        if(index > size)
        {
            std::ostringstream oss;
            oss << "Index (" << index << ") must be less or equal than "
                << "the axis size (" << size << ").";

            throw std::out_of_range(oss.str());
        }

        result = index;
    }

    return result;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t sanitize_slice_index(begin_tag, std::size_t) noexcept
{
    return 0;
}

XMIPP4_INLINE_CONSTEXPR
std::size_t sanitize_slice_index(end_tag, std::size_t size) noexcept
{
    return size;
}

#if !defined(_MSC_VER) // FIXME: Getting C1001 INTERNAL COMPILER ERROR on MSVC
template <typename I>
XMIPP4_INLINE_CONSTEXPR
std::size_t sanitize_slice_index(std::integral_constant<I, begin()>, 
                                 std::size_t ) noexcept
{
    return 0;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR
std::size_t sanitize_slice_index(std::integral_constant<I, end()>, 
                                 std::size_t size ) noexcept
{
    return size;
}
#else
template <typename I, I value>
inline
std::size_t sanitize_slice_index(std::integral_constant<I, value>, 
                                 std::size_t size ) noexcept
{
    return sanitize_slice_index(value, size); // Fallback to the normal version
}
#endif

template <typename I, I value>
inline
std::size_t sanitize_slice_index(std::integral_constant<I, value>, 
                                 std::size_t size )
{
    return sanitize_slice_index(value, size);
}

} // namespace detail

template <typename T>
std::size_t sanitize_slice_index(T index, 
                                 std::size_t size )
{
    return detail::sanitize_slice_index(index, size);
}

namespace detail
{

inline 
void check_direct_slice_ordering(std::size_t start, std::size_t stop)
{
    if(start > stop)
    {
        std::ostringstream oss;
        oss << "start value (" << start << ") must be less or equal than" 
            << " the stop (" << stop << ") value when using positive step";
        throw std::invalid_argument(oss.str());
    }
}

inline 
void check_reversed_slice_ordering(std::size_t start, std::size_t stop)
{
    if(start < stop)
    {
        std::ostringstream oss;
        oss << "start value (" << start << ") must be greater or equal than" 
            << " the stop (" << stop << ") value when using negative step";
        throw std::invalid_argument(oss.str());
    }
}

template <typename I>
inline
typename std::enable_if<std::is_integral<I>::value && std::is_signed<I>::value, std::ptrdiff_t>::type
sanitize_slice_step(I step, std::size_t start, std::size_t stop)
{
    if (step < 0)
    {
        check_reversed_slice_ordering(start, stop);
    }
    else if(step > 0)
    {
        check_direct_slice_ordering(start, stop);
    }
    else // step == 0
    {
        throw std::invalid_argument("step cannot be zero");
    }
    return step;
}

template <typename I>
inline
typename std::enable_if<std::is_integral<I>::value && !std::is_signed<I>::value, std::ptrdiff_t>::type
sanitize_slice_step(I step, std::size_t start, std::size_t stop)
{
    if(step == 0)
    {
        throw std::invalid_argument("step cannot be zero");
    }
    else // step > 0
    {
        check_direct_slice_ordering(start, stop);
    }

    return step;
}

inline
std::ptrdiff_t sanitize_slice_step(adjacent_tag, std::size_t start, std::size_t stop)
{
    check_direct_slice_ordering(start, stop);
    return 1;
}

template <typename I, I value>
inline
typename std::enable_if<(value>0), std::ptrdiff_t>::type
sanitize_slice_step(std::integral_constant<I, value>, 
                    std::size_t start, 
                    std::size_t stop)
{
    check_direct_slice_ordering(start, stop);
    return value;
}

template <typename I, I value>
inline
typename std::enable_if<(value<0), std::ptrdiff_t>::type
sanitize_slice_step(std::integral_constant<I, value>, 
                    std::size_t start, 
                    std::size_t stop)
{
    check_reversed_slice_ordering(start, stop);
    return value;
}

} // namespace detail

template <typename T>
std::ptrdiff_t sanitize_slice_step(T step, std::size_t start, std::size_t stop)
{
    return detail::sanitize_slice_step(step, start, stop);
}

template <typename Start, typename Stride, typename Stop>
void sanitize_slice(const slice<Start, Stride, Stop> &slc,
                    std::size_t size,
                    std::size_t &start,
                    std::size_t &stop,
                    std::ptrdiff_t &step )
{ 
    start = sanitize_slice_index(slc.get_start(), size);
    stop = sanitize_slice_index(slc.get_stop(), size);
    step = sanitize_slice_step(slc.get_stride(), start, stop);
}

XMIPP4_INLINE_CONSTEXPR
std::size_t compute_slice_size(std::size_t start, 
                               std::size_t stop, 
                               std::ptrdiff_t step ) noexcept
{
    if (start == stop)
    {
        return 0;
    }
    else 
    {
        if (step < 0)
        {
            return (start - stop - 1) / (-step) + 1;
        }
        else // step > 0
        {
            return (stop - start - 1) / step + 1;
        }
    }
}

} // namespace xmipp4
