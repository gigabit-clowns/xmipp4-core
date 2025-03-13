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
 * @file strided_layout_utils.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of strided_layout_utils.hpp
 * @date 2023-08-16
 * 
 */

#include "strided_layout_utils.hpp"

#include <algorithm>
#include <functional>
#include <numeric>

namespace xmipp4
{
namespace multidimensional
{

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20
ForwardIt find_first_significant_axis(ForwardIt first, 
                                      ForwardIt last )
{
    return std::find_if(first, last, is_significant);
}

template<typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
void sort_layout_inplace(BidirIt first, BidirIt last)
{
    std::sort(first, last, compare_strides_greater);
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool is_layout_sorted(ForwardIt first, ForwardIt last)
{
    first = find_first_significant_axis(first, last);

    bool result = true;
    if (first != last)
    {
        auto prev = first;
        first = find_first_significant_axis(std::next(first), last);
        while (first != last)
        {
            if (compare_strides_greater(*first, *prev))
            {
                result = false;
                break;
            }

            prev = first;
            first = find_first_significant_axis(std::next(first), last);
        }
    }

    return result;
}



namespace detail
{

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt ravel_layout_one(ForwardIt first, 
                           ForwardIt last,
                           axis_descriptor &ravelled,
                           std::ptrdiff_t &offset ) noexcept
{
    std::size_t extent = first->get_extent();
    offset -= get_axis_pivot_offset(*first);

    auto prev = first;
    first = find_first_significant_axis(std::next(first), last);
    while(first != last && is_mirror_contiguous(*first, *prev))
    {
        extent *= first->get_extent();
        offset -= get_axis_pivot_offset(*first);

        prev = first;
        first = find_first_significant_axis(std::next(first), last);
    }

    const auto stride = prev->get_unsigned_stride();
    ravelled = axis_descriptor(extent, stride);
    return first;
}

} // namespace detail

template<typename ForwardIt, typename OutputIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
OutputIt ravel_layout(ForwardIt first_from, 
                      ForwardIt last_from,
                      OutputIt first_to,
                      std::ptrdiff_t &offset )
{
    // Start at a significant axis
    first_from = find_first_significant_axis(first_from, last_from);

    // Ravel contiguous runs of axes
    while(first_from != last_from)
    {
        // Ravel a single run of the layout
        axis_descriptor ravelled;
        first_from = detail::ravel_layout_one(
            first_from, last_from, 
            ravelled, offset
        );

        // Write a new axis to the output
        *first_to = ravelled;
        ++first_to;
    }

    return first_to;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt ravel_layout_inplace(ForwardIt first, 
                              ForwardIt last,
                              std::ptrdiff_t &offset )
{
    // It is safe to call the non-inplace version,
    // as it will write to already read positions.
    return ravel_layout(first, last, first, offset);
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
std::size_t compute_contiguous_axis_strides(ForwardIt first,
                                            ForwardIt last ) noexcept
{
    std::size_t volume = 1;
    std::for_each(
        first, last,
        [&volume] (axis_descriptor& desc) -> void
        {
            desc.set_stride(volume);
            volume *= desc.get_extent();
        }
    );
    return volume;
}

template <typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
void transpose_layout_inplace(BidirIt first, BidirIt last) noexcept
{
    std::reverse(first, last);
}

template <typename BidirIt, typename OutputIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
OutputIt transpose_layout(BidirIt first_from, BidirIt last_from,
                          OutputIt first_to )
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
        std::size_t(1),
        [] (std::size_t current, const axis_descriptor &axis) -> std::size_t
        {
            return current * axis.get_extent();
        }
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
std::size_t compute_layout_storage_size(ForwardIt first,
                                        ForwardIt last ) noexcept
{
    std::ptrdiff_t result = 0;
    while(first != last)
    {
        std::ptrdiff_t last_offset;
        if (get_axis_last_offset(*first, last_offset))
        {
            result += math::abs(last_offset);
        }
        else
        {
            // Zero sized axis found, no elements can be stored 
            // in this layout.
            return 0;
        }

        ++first;
    }

    return result + 1;
}

template <typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt squeeze_layout_inplace(ForwardIt first, ForwardIt last) noexcept
{
    return std::remove_if(
        first, last,
        check_squeeze
    );
}

template <typename InputIt, typename OutputIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
OutputIt squeeze_layout(InputIt first_from, 
                        InputIt last_from,
                        OutputIt first_to )
{
    return std::remove_copy_if(
        first_from, last_from,
        first_to,
        check_squeeze
    );
}

template <typename... ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool broadcast_layouts(std::size_t rank,
                       ForwardIt... firsts )
{
    bool result = true;

    for(std::size_t i = 0; i < rank; ++i)
    {
        if(!broadcast((*(firsts++))...))
        {
            // Failed to broadcast the current dimension
            result = false;
            break;
        }
    }

    return result;
}

} // namespace multidimensional
} // namespace xmipp4
