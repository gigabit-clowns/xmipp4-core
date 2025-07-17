// SPDX-License-Identifier: GPL-3.0-only

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



namespace detail
{


template<typename ForwardIt, typename Cmp>
XMIPP4_INLINE_CONSTEXPR_CPP20
ForwardIt find_significant_min_stride(ForwardIt first, 
                                      ForwardIt last, 
                                      const Cmp &compare )
{
    // Start at the first non-zero stride
    first = find_first_significant_axis(first, last);

    ForwardIt result = first;
    if (first != last)
    {
        first = find_first_significant_axis(std::next(first), last);
        while(first != last)
        {
            if(compare(*first, *result))
                result = first;

            first = find_first_significant_axis(std::next(first), last);
        }
    }

    return result;
}

} // namespace detail

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20
ForwardIt find_major_axis(ForwardIt first, ForwardIt last) noexcept
{
    return detail::find_significant_min_stride(
        first, last,
        compare_strides_less
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt find_minor_axis(ForwardIt first, ForwardIt last) noexcept
{
    return detail::find_significant_min_stride(
        first, last,
        compare_strides_greater
    );
}

template<typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
void sort_layout_inplace(BidirIt first, 
                         BidirIt last,
                         column_major_tag )
{
    std::sort(first, last, compare_strides_greater);
}

template<typename BidirIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
void sort_layout_inplace(BidirIt first, 
                         BidirIt last,
                         row_major_tag )
{
    std::sort(first, last, compare_strides_less);
}



namespace detail
{

template<typename ForwardIt, typename Cmp>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool check_layout_order(ForwardIt first, 
                        ForwardIt last,
                        const Cmp &compare ) noexcept
{
    // Start at the first non-zero stride
    first = find_first_significant_axis(first, last);

    bool result = true;
    if (first != last)
    {
        auto prev = first;
        first = find_first_significant_axis(std::next(first), last);
        while(first != last)
        {
            if(!compare(*prev, *first))
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

} // namespace detail

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool check_layout_order(ForwardIt first, 
                        ForwardIt last,
                        column_major_tag ) noexcept
{
    return detail::check_layout_order(first, last, compare_strides_greater);
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool check_layout_order(ForwardIt first, 
                        ForwardIt last,
                        row_major_tag ) noexcept
{
    return detail::check_layout_order(first, last, compare_strides_less);
}



namespace detail
{

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt pack_layout_one(ForwardIt first, 
                          ForwardIt last,
                          axis_descriptor &packed,
                          std::ptrdiff_t &offset,
                          column_major_tag ) noexcept
{
    std::size_t extent = first->get_extent();
    offset -= get_reverse_axis_offset(*first);

    auto prev = first;
    first = find_first_significant_axis(std::next(first), last);
    while(first != last && is_regular(*first, *prev))
    {
        extent *= first->get_extent();
        offset -= get_reverse_axis_offset(*first);

        prev = first;
        first = find_first_significant_axis(std::next(first), last);
    }

    const auto stride = prev->get_unsigned_stride();
    packed = axis_descriptor(extent, stride);
    return first;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt pack_layout_one(ForwardIt first, 
                          ForwardIt last,
                          axis_descriptor &packed,
                          std::ptrdiff_t &offset,
                          row_major_tag ) noexcept
{
    std::size_t extent = first->get_extent();
    const auto stride = first->get_unsigned_stride();
    offset -= get_reverse_axis_offset(*first);

    auto prev = first;
    first = find_first_significant_axis(std::next(first), last);
    while(first != last && is_regular(*prev, *first))
    {
        extent *= first->get_extent();
        offset -= get_reverse_axis_offset(*first);

        prev = first;
        first = find_first_significant_axis(std::next(first), last);
    }

    packed = axis_descriptor(extent, stride);
    return first;
}

} // namespace detail

template<typename ForwardIt, typename OutputIt, typename OrderTag>
XMIPP4_INLINE_CONSTEXPR_CPP20 
OutputIt pack_layout(ForwardIt first_from, 
                     ForwardIt last_from,
                     OutputIt first_to,
                     std::ptrdiff_t &offset,
                     OrderTag &&order )
{
    // Start at a significant axis
    first_from = find_first_significant_axis(first_from, last_from);

    // Pack contiguous runs of axes
    while(first_from != last_from)
    {
        // Pack a single run of the layout
        axis_descriptor packed;
        first_from = detail::pack_layout_one(
            first_from, last_from, 
            packed, offset,
            std::forward<OrderTag>(order)
        );

        // Write a new axis to the output
        *first_to = packed;
        ++first_to;
    }

    return first_to;
}

template<typename ForwardIt, typename OrderTag>
XMIPP4_INLINE_CONSTEXPR_CPP20 
ForwardIt pack_layout_inplace(ForwardIt first, 
                              ForwardIt last,
                              std::ptrdiff_t &offset,
                              OrderTag &&order )
{
    // It is safe to call the non-inplace version,
    // as it will write to already read positions.
    return pack_layout(
        first, last, 
        first, 
        offset, 
        std::forward<OrderTag>(order)
    );
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
bool is_contiguous_layout(ForwardIt first, ForwardIt last)
{
    const auto slow_axis = find_minor_axis(first, last);
    const auto expected_size = (slow_axis != last) ? get_axis_length(*slow_axis) : 1;
    const auto size = compute_layout_buffer_size(first, last);
    return size == expected_size;
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
std::ptrdiff_t compute_layout_last_position(ForwardIt first,
                                            ForwardIt last ) noexcept
{
    std::ptrdiff_t result = 0;
    while(first != last)
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
            result = -1;
            break;
        }

        ++first;
    }

    return result;
}

template<typename ForwardIt>
XMIPP4_INLINE_CONSTEXPR_CPP20 
std::size_t compute_layout_buffer_size(ForwardIt first,
                                       ForwardIt last ) noexcept
{
    return compute_layout_last_position(first, last) + 1;
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

} // namespace multidimensional
} // namespace xmipp4
