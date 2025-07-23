// SPDX-License-Identifier: GPL-3.0-only

#include "slice.hpp"

#include <sstream>
#include <exception>

namespace xmipp4 
{
namespace multidimensional
{

template <typename Start, typename Count, typename Step>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Count, Step>::slice(start_type start, 
                                 count_type count,
                                 step_type step ) noexcept
    : m_start(start)
    , m_count(count)
    , m_step(step)
{
}

template <typename Start, typename Count, typename Step>
template <typename Start2, typename Count2, typename Step2>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Count, Step>::slice(Start2 start,
                                 Count2 count,
                                 Step2 step ) noexcept
    : slice(
        static_cast<start_type>(start),
        propagate_end<count_type>(count),
        static_cast<step_type>(step)
    )
{
}

template <typename Start, typename Count, typename Step>
template <typename Start2, typename Count2, typename Step2>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Count, Step>::slice(const slice<Start2, Count2, Step2>& other) noexcept
    : slice(other.get_start(), other.get_count(), other.get_step())
{
}

template <typename Start, typename Count, typename Step>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Count, Step>::set_start(start_type start) noexcept
{
    m_start = start;
}

template <typename Start, typename Count, typename Step>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Count, Step>::start_type& 
slice<Start, Count, Step>::get_start() const noexcept
{
    return m_start;
}

template <typename Start, typename Count, typename Step>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Count, Step>::set_count(count_type count) noexcept
{
    m_count = count;
}

template <typename Start, typename Count, typename Step>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Count, Step>::count_type& 
slice<Start, Count, Step>::get_count() const noexcept
{
    return m_count;
}

template <typename Start, typename Count, typename Step>
XMIPP4_INLINE_CONSTEXPR void 
slice<Start, Count, Step>::set_step(step_type step) noexcept
{
    m_step = step;
}

template <typename Start, typename Count, typename Step>
XMIPP4_INLINE_CONSTEXPR const typename slice<Start, Count, Step>::step_type&
slice<Start, Count, Step>::get_step() const noexcept
{
    return m_step;
}



template <typename Start1, typename Count1, typename Step1, 
          typename Start2, typename Count2, typename Step2>
XMIPP4_INLINE_CONSTEXPR bool
operator==( const slice<Start1, Count1, Step1>& lhs, 
            const slice<Start2, Count2, Step2>& rhs ) noexcept
{
    return lhs.get_start() == rhs.get_start() &&
           lhs.get_count() == rhs.get_count() &&
           lhs.get_step() == rhs.get_step();
}

template <typename Start1, typename Count1, typename Step1, 
          typename Start2, typename Count2, typename Step2>
XMIPP4_INLINE_CONSTEXPR bool
operator!=( const slice<Start1, Count1, Step1>& lhs, 
            const slice<Start2, Count2, Step2>& rhs ) noexcept
{
    return !(lhs == rhs);
}

template <typename Start, typename Count, typename Step>
inline std::ostream& 
operator<<(std::ostream& os, const slice<Start, Count, Step> &s)
{
    return os << "slice(" << s.get_start()
              << ", " << s.get_count()
              << ", " << s.get_step() << ")";
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



namespace detail
{

template <typename To, typename From>
XMIPP4_INLINE_CONSTEXPR
typename std::enable_if<std::is_same<From, To>::value, To>::type
propagate_end(From x)
{
    return x;
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
To propagate_end(end_tag)
{
    return static_cast<To>(end());
}

template <typename To>
XMIPP4_INLINE_CONSTEXPR
To propagate_end(begin_tag)
{
    return static_cast<To>(begin());
}

template <typename To, typename From, From value>
XMIPP4_INLINE_CONSTEXPR
To propagate_end(std::integral_constant<From, value>)
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
    return false;
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



XMIPP4_INLINE_CONSTEXPR reversed_tag reversed() noexcept
{
    return reversed_tag();
}

XMIPP4_INLINE_CONSTEXPR bool
operator==(const reversed_tag&, const reversed_tag&) noexcept
{
    return true;
}

XMIPP4_INLINE_CONSTEXPR bool
operator!=(const reversed_tag&, const reversed_tag&) noexcept
{
    return false;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(const reversed_tag& lhs, I rhs) noexcept
{
    return static_cast<I>(lhs) == rhs;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(const reversed_tag& lhs, I rhs) noexcept
{
    return static_cast<I>(lhs) != rhs;
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(I lhs, const reversed_tag& rhs) noexcept
{
    return lhs == static_cast<I>(rhs);
}

template <typename I>
XMIPP4_INLINE_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(I lhs, const reversed_tag& rhs) noexcept
{
    return lhs != static_cast<I>(rhs);
}

inline std::ostream& operator<<(std::ostream& os, reversed_tag)
{
    return os << "reversed";
}



XMIPP4_INLINE_CONSTEXPR all_slice all() noexcept
{
    return all_slice();
}

XMIPP4_INLINE_CONSTEXPR even_slice even() noexcept
{
    return even_slice();
}

XMIPP4_INLINE_CONSTEXPR odd_slice odd() noexcept
{
    return odd_slice();
}



template <typename Count>
XMIPP4_INLINE_CONSTEXPR 
slice<begin_tag, Count, adjacent_tag> 
make_slice(Count count) noexcept
{
    return slice<begin_tag, Count, adjacent_tag>(begin(), count, adjacent());
}

template <typename Start, typename Count>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Count, adjacent_tag> 
make_slice(Start start, Count count) noexcept
{
    return slice<Start, Count, adjacent_tag>(start, count, adjacent());
}

template <typename Start, typename Count, typename Step>
XMIPP4_INLINE_CONSTEXPR 
slice<Start, Count, Step> 
make_slice(Start start, Count count, Step step) noexcept
{
    return slice<Start, Count, Step>(start, count, step);
}





namespace detail
{

inline
void sanitize_slice_start(dynamic_slice &slice, std::size_t extent)
{
    const auto start = slice.get_start();
    const auto count = slice.get_count();

    if (count > 0)
    {
        if (start < -static_cast<std::ptrdiff_t>(extent) || 
            start >= static_cast<std::ptrdiff_t>(extent) )
        {
            std::ostringstream oss;
            oss << "Non-empty slice's start index " << start 
                << " is out of bounds for extent " << extent << ".";
            throw std::out_of_range(oss.str());
        }

        if (start < 0)
        {
            slice.set_start(start + extent);
        }
    }
    else
    {
        if (start < 0)
        {
            slice.set_start(0);
        }
    }
}

inline
void sanitize_slice_count(dynamic_slice &slice, std::size_t extent)
{
    const auto start = slice.get_start();
    const auto count = slice.get_count();
    const auto step = slice.get_step();

    if (step > 0)
    {
        if (count == end())
        {
            const auto new_count = (extent - start + step - 1) / step;
            slice.set_count(new_count);
        }
        else if (count > 0U && start + step*(count-1) >= extent)
        {
            std::ostringstream oss;
            oss << "Slice count " << count 
                << " start index " << start
                << " and step " << step 
                << " overflows extent " << extent;
            throw std::out_of_range(oss.str());
        }
    }
    else // step < 0
    {
        const auto abs_step = -step;
        if (count == end())
        {
            const auto new_count = start / abs_step + 1;
            slice.set_count(new_count);
        }
        else if (count > 0U && abs_step*static_cast<std::ptrdiff_t>(count-1) > start)
        {
            std::ostringstream oss;
            oss << "Reversed slice with count " << count 
                << " start index " << start
                << " and step " << step 
                << " underflows 0";
            throw std::out_of_range(oss.str());
        }
    }
}

inline
void sanitize_slice_step(dynamic_slice &slice)
{
    if (slice.get_step() == 0)
    {
        throw std::invalid_argument("Slice step cannot be zero.");
    }
}

} // namespace detail

template <typename Start, typename Count, typename Step>
inline
slice<std::size_t, std::size_t, std::ptrdiff_t>
sanitize_slice(const slice<Start, Count, Step>& slice, std::size_t extent)
{
    dynamic_slice result(slice);

    detail::sanitize_slice_step(result);
    detail::sanitize_slice_start(result, extent);
    detail::sanitize_slice_count(result, extent);

    return result;
}

} // namespace multidimensional
} // namespace xmipp4
