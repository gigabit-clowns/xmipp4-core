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
 * @file memory_layout.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of memory_layout.hpp
 * @date 2023-08-16
 * 
 */

#include "memory_layout.hpp"
#include "../platform/cpp_execution.hpp"

#include <algorithm>
#include <numeric>

namespace xmipp4
{

XMIPP4_INLINE_CONSTEXPR 
bool check_axis_order(const axis_descriptor &prev, 
                      const axis_descriptor &next,
                      column_major_tag ) noexcept
{
    return prev.get_unsigned_step() < next.get_unsigned_step();
}

XMIPP4_INLINE_CONSTEXPR 
bool check_axis_order(const axis_descriptor &prev, 
                      const axis_descriptor &next,
                      row_major_tag ) noexcept
{
    return prev.get_unsigned_step() > next.get_unsigned_step();
}

XMIPP4_INLINE_CONSTEXPR 
bool check_axis_overlap(const axis_descriptor &major, 
                        const axis_descriptor &minor ) noexcept
{
    return minor.get_unsigned_step() < major.get_width();
}

XMIPP4_INLINE_CONSTEXPR 
bool is_regular(const axis_descriptor &major, 
                const axis_descriptor &minor ) noexcept
{
    return major.get_width() == minor.get_unsigned_step();
}

template<typename ForwardIt, typename OrderTag>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool check_layout_order(ForwardIt first, 
                        ForwardIt last,
                        OrderTag &&order) noexcept
{
    return std::is_sorted(XMIPP4_PAR_UNSEQ
        first, last, 
        [order] (const auto &x, const auto &y) -> bool
        {
            return check_axis_order(x, y, order);
        }
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool check_layout_overlap(ForwardIt first, 
                          ForwardIt last,
                          column_major_tag ) noexcept

{
    const auto ite = std::adjacent_find(XMIPP4_PAR_UNSEQ
        first, last,
        [] (const axis_descriptor &prev, const axis_descriptor &next) -> bool
        {
            return check_axis_overlap(prev, next);
        }
    );

    return ite != last;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool check_layout_overlap(ForwardIt first, 
                          ForwardIt last,
                          row_major_tag ) noexcept
{
    const auto ite = std::adjacent_find(XMIPP4_PAR_UNSEQ
        first, last,
        [] (const axis_descriptor &prev, const axis_descriptor &next) -> bool
        {
            return check_axis_overlap(next, prev);
        }
    );

    return ite != last;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool is_regular_layout(ForwardIt first, 
                       ForwardIt last,
                       column_major_tag ) noexcept
{
    const auto ite = std::adjacent_find(XMIPP4_PAR_UNSEQ
        first, last,
        [] (const axis_descriptor &prev, const axis_descriptor &next) -> bool
        {
            return !is_regular(prev, next);
        }
    );

    return ite == last;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool is_regular_layout(ForwardIt first, 
                       ForwardIt last,
                       row_major_tag ) noexcept
{
    const auto ite = std::adjacent_find(XMIPP4_PAR_UNSEQ
        first, last,
        [] (const axis_descriptor &prev, const axis_descriptor &next) -> bool
        {
            return !is_regular(next, prev);
        }
    );

    return ite == last;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool is_contiguous_layout(ForwardIt first, 
                          ForwardIt last,
                          column_major_tag ) noexcept
{
    // Check that it is not an empty range
    if(first == last)
        return true;
    
    // The first axis must have unit step
    if(first->get_step() != 1)
        return false;

    // Layout must be regular
    if(!is_regular_layout(first, last, column_major))
        return false;

    return true;
}

template<typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool is_contiguous_layout(BidirIt first, 
                          BidirIt last,
                          row_major_tag ) noexcept
{
    // Check that it is not an empty range
    if(first == last)
        return true;
    
    // Layout must be regular
    if(!is_regular_layout(first, last, row_major))
        return false;

    // The last axis must have unit step
    if(std::make_reverse_iterator(last)->get_step() != 1)
        return false;

    return true;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
std::size_t compute_contiguous_axis_steps(ForwardIt first,
                                          ForwardIt last,
                                          column_major_tag) noexcept
{
    std::size_t volume = 1;
    std::for_each(XMIPP4_SEQ
        first, last,
        [&volume] (axis_descriptor& desc) -> void
        {
            desc.set_step(volume);
            volume *= desc.get_count();
        }
    );
    return volume;
}

template<typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
std::size_t compute_contiguous_axis_steps(BidirIt first,
                                          BidirIt last,
                                          row_major_tag) noexcept
{
    return compute_contiguous_axis_steps(
        std::make_reverse_iterator(last),
        std::make_reverse_iterator(first),
        column_major
    );
}

template<typename InputIt, typename OutputIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
OutputIt fill_shape_from_axes(InputIt first, 
                              InputIt last,
                              OutputIt out )
{
    return std::transform(
        first, last,
        out,
        std::mem_fn(&axis_descriptor::get_count)
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt find_max_step(ForwardIt first, ForwardIt last) noexcept
{
    return std::max_element(XMIPP4_PAR_UNSEQ
        first, last, 
        [] (const axis_descriptor &x, const axis_descriptor &y) -> bool
        {
            return check_axis_order(x, y, column_major);
        }
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20
ForwardIt find_min_step(ForwardIt first, ForwardIt last) noexcept
{
    return std::min_element(XMIPP4_PAR_UNSEQ
        first, last, 
        [] (const axis_descriptor &x, const axis_descriptor &y) -> bool
        {
            return check_axis_order(x, y, column_major);
        }
    );
}

template <typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
void transpose_layout(BidirIt first, BidirIt last) noexcept
{
    std::reverse(first, last);
}

template <typename BidirIt, typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt transpose_layout(BidirIt first_from, BidirIt last_from,
                           ForwardIt first_to )
{
    return std::reverse_copy(
        first_from, last_from,
        first_to
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
std::size_t compute_layout_volume(ForwardIt first,
                                  ForwardIt last ) noexcept
{
    return std::accumulate(
        first, last,
        1UL,
        [] (std::size_t current, const axis_descriptor &axis) -> std::size_t
        {
            return current * axis.get_count();
        }
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
axis_descriptor flatten_regular_layout(ForwardIt first,
                                       ForwardIt last,
                                       column_major_tag ) noexcept
{
    if (first == last)
        return axis_descriptor();

    const auto step = first->get_step();
    const auto count = compute_layout_volume(first, last);

    return axis_descriptor(count, step);
}   

template<typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
axis_descriptor flatten_regular_layout(BidirIt first,
                                       BidirIt last,
                                       row_major_tag ) noexcept
{
    if (first == last)
        return axis_descriptor();

    const auto step = std::make_reverse_iterator(last)->get_step();
    const auto count = compute_layout_volume(first, last);

    return axis_descriptor(count, step);
}


XMIPP4_INLINE_CONSTEXPR 
bool check_squeeze(const axis_descriptor &axis) noexcept
{
    return axis.get_count() == 1;
}

template <typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt squeeze_layout(ForwardIt first, ForwardIt last) noexcept
{
    return std::remove_if(
        first, last,
        check_squeeze
    );
}

template <typename ForwardIt1, typename ForwardIt2>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt2 squeeze_layout(ForwardIt1 first_from, 
                          ForwardIt1 last_from,
                          ForwardIt2 first_to )
{
    return std::remove_copy_if(
        first_from, last_from,
        first_to,
        check_squeeze    
    );
}

} // namespace xmipp4
