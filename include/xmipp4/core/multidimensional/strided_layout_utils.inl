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
ForwardIt find_max_stride(ForwardIt first, ForwardIt last) noexcept
{
    return std::max_element(
        first, last, 
        compare_strides_less
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20
ForwardIt find_min_stride(ForwardIt first, ForwardIt last) noexcept
{
    return std::min_element(
        first, last, 
        compare_strides_less
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt find_first_significant_axis(ForwardIt first, ForwardIt last) noexcept
{
    auto result = std::find_if(
        first, last,
        is_significant
    );

    if(result != last)
    {
        for(auto ite = std::next(result); ite != last; ++ite)
        {
            if(is_significant(*ite) &&
               compare_strides_less(*ite, *result) )
            {
                result = ite;
            }
        }
    }

    return result;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt find_next_significant_axis(ForwardIt current,
                                     ForwardIt first,
                                     ForwardIt last ) noexcept
{
    // Find the first stride greater than current one
    auto result = std::find_if(
        first, last,
        [current] (const axis_descriptor &desc)
        {
            return !check_squeeze(desc) && compare_strides_less(*current, desc);
        }
    );

    // Try to minimize the stride
    if (result != last)
    {
        for(auto ite = std::next(result); ite != last; ++ite)
        {
            // current < first < result
            if (!check_squeeze(*ite) &&
                compare_strides_less(*current, *ite) &&
                compare_strides_less(*ite, *result) )
            {
                result = ite;
            }
        }
    }

    return result;
}

namespace detail
{

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt pack_layout_one(ForwardIt first, 
                          ForwardIt last,
                          ForwardIt current,
                          axis_descriptor &packed,
                          std::ptrdiff_t &offset ) noexcept
{
    std::size_t extent = current->get_extent();
    const auto stride = current->get_unsigned_stride();
    offset -= get_reverse_axis_offset(*current);

    auto prev = current;
    current = find_next_significant_axis(current, first, last);
    while(current != last && is_packed(*prev, *current))
    {
        extent *= current->get_extent();
        offset -= get_reverse_axis_offset(*current);

        prev = current;
        current = find_next_significant_axis(current, first, last);
    }

    packed = axis_descriptor(extent, stride);

    return current;
}

} // namespace detail

template<typename ForwardIt, typename OutputIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
OutputIt pack_layout(ForwardIt first_from, 
                     ForwardIt last_from,
                     OutputIt first_to,
                     std::ptrdiff_t &offset )
{
    auto ite = find_first_significant_axis(first_from, last_from);

    // Pack contiguous runs of axes
    while(ite != last_from)
    {
        // Pack a single run of the layout
        axis_descriptor packed;
        ite = detail::pack_layout_one(
            first_from, last_from, 
            ite,
            packed, offset
        );

        // Write a new axis to the output
        *first_to = packed;
        ++first_to;
    }

    return first_to;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
std::size_t compute_contiguous_axis_strides(ForwardIt first,
                                            ForwardIt last,
                                            column_major_tag) noexcept
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

template<typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
std::size_t compute_contiguous_axis_strides(BidirIt first,
                                            BidirIt last,
                                            row_major_tag) noexcept
{
    return compute_contiguous_axis_strides(
        std::make_reverse_iterator(last),
        std::make_reverse_iterator(first),
        column_major()
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
        std::mem_fn(&axis_descriptor::get_extent)
    );
}



template <typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
void transpose_layout(BidirIt first, BidirIt last) noexcept
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
std::size_t compute_layout_buffer_size(ForwardIt first,
                                       ForwardIt last ) noexcept
{
    std::size_t result = 0;
    for(; first != last; ++first)
    {
        const auto last_position = get_axis_last_position(*first);
        if (last_position >= 0)
        {
            result += last_position;
        }
        else
        {
            // Zero sized axis found, no elements can be stored 
            // in this layout.
            result = 0;
            break;
        }
    }

    // If no zero sized axis was found (reached the end), 
    // add 1 to the result
    return result + static_cast<std::size_t>(first==last);
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


namespace detail
{

template<typename InputIt, typename OutputIt>
inline
OutputIt apply_subscripts_to_layout(InputIt first, 
                                    InputIt last,
                                    OutputIt out,
                                    const subscript_sequence<>&,
                                    std::ptrdiff_t &)
{
    // No more subscripts to be processed.
    // Copy the remaining axes
    return std::copy(first, last, out);
}

template<typename InputIt, typename OutputIt, typename I, typename... Subscripts>
inline
typename std::enable_if<is_index<I>::value, OutputIt>::type
apply_subscripts_to_layout(InputIt first, 
                           InputIt last,
                           OutputIt out,
                           const subscript_sequence<I, Subscripts...>& subscripts,
                           std::ptrdiff_t &offset)
{
    // Consume index
    apply_index(*first, subscripts.head(), offset);
    ++first;

    return apply_subscripts_to_layout(
        first, last,
        out,
        subscripts.tail(),
        offset
    );
}

template<typename InputIt, typename OutputIt, typename S, typename... Subscripts>
typename std::enable_if<is_slice<S>::value, OutputIt>::type
apply_subscripts_to_layout(InputIt first, 
                           InputIt last,
                           OutputIt out,
                           const subscript_sequence<S, Subscripts...>& subscripts,
                           std::ptrdiff_t &offset)
{
    *out = apply_slice(*first, subscripts.head(), offset);
    ++out;
    ++first;

    return apply_subscripts_to_layout(
        first, last,
        out,
        subscripts.tail(),
        offset
    );
}

template<typename InputIt, typename OutputIt, typename... Subscripts>
inline
OutputIt apply_subscripts_to_layout(InputIt first, 
                                    InputIt last,
                                    OutputIt out,
                                    const subscript_sequence<new_axis_tag, Subscripts...>& subscripts,
                                    std::ptrdiff_t &offset )
{
    *out = make_phantom_axis(); // Add an axis
    ++out;

    return apply_subscripts_to_layout(
        first, last,
        out,
        subscripts.tail(),
        offset
    );
}

template<typename InputIt, typename OutputIt, typename... Subscripts>
inline
OutputIt apply_subscripts_to_layout(InputIt first, 
                                    InputIt last,
                                    OutputIt out,
                                    const subscript_sequence<ellipsis_tag, Subscripts...>& subscripts,
                                    std::ptrdiff_t &offset )
{
    using tail_type = typename decltype(subscripts)::tail_type;
    XMIPP4_CONST_CONSTEXPR auto remaining_subscripts = tail_type::consumption();
    const auto remaining_axes = std::distance(first, last);
    const auto padding_axes = remaining_axes - remaining_subscripts;
    
    // Copy padding axes
    for (decltype(padding_axes) i = 0; i < padding_axes; ++i, ++first, ++out)
    {
        *out = *first;
    }

    return apply_subscripts_to_layout(
        first, last,
        out,
        subscripts.tail(),
        offset
    );
}

} // namespace detail

template<typename InputIt, typename OutputIt, typename... Subscripts>
inline
OutputIt apply_subscripts_to_layout(InputIt first, 
                                    InputIt last,
                                    OutputIt out,
                                    const subscript_sequence<Subscripts...>& subscripts,
                                    std::ptrdiff_t &offset )
{
    return detail::apply_subscripts_to_layout(
        first, last, 
        out,
        subscripts, 
        offset
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
