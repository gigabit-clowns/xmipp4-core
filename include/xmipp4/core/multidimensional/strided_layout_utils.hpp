#pragma once

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
 * @file strided_layout_utils.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Declaration of utility functions for handling sequences 
 * of axis_descriptor
 * @date 2023-10-16
 * 
 */

#include "axis_descriptor.hpp"
#include "layout_flags.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Find the first significant.
 * 
 * Finds the first item in the range [first, last) where is_significant
 * returns true. If none is found, last is returned.
 * 
 * @tparam ForwardIt Forward iterator.
 * @param first Iterator to the first element in the range.
 * @param last Iterator to the past-the-end element in the range.
 * @return ForwardIt Iterator to the first significant axis.
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
ForwardIt find_first_significant_axis(ForwardIt first, 
                                      ForwardIt last );

/**
 * @brief Finds the significant axis with the smallest stride magnitude.
 * 
 * If there are no significant axes in the range, last is returned.
 * 
 * @tparam ForwardIt Forward iterator.
 * @param first Iterator to the first element in the range.
 * @param last Iterator to the past-the-end element in the range.
 * @return ForwardIt The axis with the smallest stride magnitude.
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 ForwardIt find_major_axis(ForwardIt first, 
                                                 ForwardIt last ) noexcept;

/**
 * @brief Finds the significant axis with the largest stride magnitude.
 * 
 * If there are no significant axes in the range, last is returned.
 * 
 * @tparam ForwardIt Forward iterator.
 * @param first Iterator to the first element in the range.
 * @param last Iterator to the past-the-end element in the range.
 * @return ForwardIt The axis with the largest stride magnitude.
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 ForwardIt find_minor_axis(ForwardIt first, 
                                                 ForwardIt last ) noexcept;

/**
 * @brief Sort the layout so that it is column major.
 * 
 * After this operation, the provided layout's axes will be ordered 
 * with increasing strides.
 * 
 * @tparam BidirIt Bidirectional iterator.
 * @param first Iterator to the first element in the range.
 * @param last Iterator to the past-the-end element in the range.
 */
template<typename BidirIt>
XMIPP4_CONSTEXPR_CPP20 void sort_layout_inplace(BidirIt first, 
                                                BidirIt last );

/**
 * @brief Check if the layout is sorted in column major.
 * 
 * Note that ordering is only tested for significant axes.
 * 
 * @tparam ForwardIt Forward iterator.
 * @param first Iterator to the first axis of the layout.
 * @param last Iterator to the past-the-end axis of the layout.
 * @return bool True if the layout is sorted.
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
bool is_layout_sorted(ForwardIt first, ForwardIt last);

/**
 * @brief Obtain the flags of this layout.
 * 
 * @tparam ForwardIt Forward iterator.
 * @param first Iterator to the first axis of the layout.
 * @param last Iterator to the past-the-end axis of the layout.
 * @return layout_flags The flags.
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
layout_flags compute_layout_flags(ForwardIt first, ForwardIt last);

/**
 * @brief Merge contiguous axes of a layout to reduce it as much as possible.
 * 
 * @tparam ForwardIt Forward iterator.
 * @tparam OutputIt Output iterator.
 * @param first_from Iterator to the first input axis.
 * @param last_from Iterator to the past-the-end input axis.
 * @param first_to Iterator to the first output axis.
 * @param offset Offset to be modified to account negative strides.
 * @param order Ordering of the axes.
 * @return OutputIt Iterator to the past-the-end output axis.
 * 
 * @note Input range must be sorted. Otherwise behavior is undefined. 
 * Output will be ordered accordingly.
 * 
 */
template<typename ForwardIt, typename OutputIt>
XMIPP4_CONSTEXPR_CPP20 OutputIt ravel_layout(ForwardIt first_from, 
                                             ForwardIt last_from,
                                             OutputIt first_to,
                                             std::ptrdiff_t &offset );

/**
 * @brief Merge contiguous axes of a layout to reduce it as much as possible.
 * 
 * This function operates in-place, meaning that axes are modified and the new
 * past-the-end iterator is returned. Items contained between the new end 
 * and the previous end should be erased.
 * 
 * @see ravel_layout
 * @tparam ForwardIt Forward iterator.
 * @tparam OrderTag Order tag.
 * @param first Iterator to the first input axis.
 * @param last Iterator to the past-the-end input axis.
 * @param offset Offset to be modified to account negative strides.
 * @return ForwardIt Iterator to the new past-the-end axis.
 * 
 * @note Input range must be sorted. Otherwise behavior is undefined. 
 * Output will be ordered accordingly.
 * 
 * 
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 ForwardIt ravel_layout_inplace(ForwardIt first, 
                                                      ForwardIt last,
                                                      std::ptrdiff_t &offset );

/**
 * @brief Check if the layout is contiguous.
 * 
 * @tparam ForwardIt Forward iterator.
 * @param first Iterator to the first axis of the layout.
 * @param last Iterator to the past-the-end axis of the layout.
 * @return bool True if the layout is contiguous.
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
bool is_contiguous_layout(ForwardIt first, ForwardIt last);

/**
 * @brief Fill the strides of a layout such that it is contiguous
 * in column major ordering
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return std::size_t The volume, this is, the product of all sizes
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
std::size_t compute_contiguous_axis_strides(ForwardIt first,
                                            ForwardIt last ) noexcept;

/**
 * @brief Reverses the ordering of a layout
 * 
 * @tparam BidirIt Bidirectional iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 */
template <typename BidirIt>
XMIPP4_CONSTEXPR_CPP20 
void transpose_layout_inplace(BidirIt first, BidirIt last) noexcept;

/**
 * @brief Reverses the ordering of a layout out-of-place
 * 
 * @tparam BidirIt Bidirectional iterator
 * @tparam OutputIt Output iterator
 * @param first_from Iterator to the first element in the input range
 * @param last_from Iterator to the past-the-end element in the input range
 * @param first_to Iterator to hte first element in the output range.
 * Must have (last-first) elements available
 */
template <typename BidirIt, typename OutputIt>
XMIPP4_CONSTEXPR_CPP20 OutputIt transpose_layout(BidirIt first_from, 
                                                 BidirIt last_from,
                                                 OutputIt first_to );

/**
 * @brief Computes the volume of a layout
 * 
 * This is equivalent to the product of all its axis extents
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return std::size_t The volume of the layout
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
std::size_t compute_layout_volume(ForwardIt first,
                                  ForwardIt last ) noexcept;

/**
 * @brief Computes the last position referenced by the layout.
 * 
 * The last position of a layout is the sum of all axes' last
 * position if none of them is -1. If at least one of them equals
 * -1, -1 will be returned to signal an empty array.
 * 
 * @see get_axis_last_position
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return std::ptrdiff_t. The last position referenced by the layout.
 * -1 if the layout has zero elements.
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
std::ptrdiff_t compute_layout_last_position(ForwardIt first,
                                            ForwardIt last ) noexcept;
/**
 * @brief Computes the buffer size required to store a layout
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return std::size_t The size of the buffer required to store the
 * provided layout. In elements.
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
std::size_t compute_layout_buffer_size(ForwardIt first,
                                       ForwardIt last ) noexcept;

/**
 * @brief Squeezes a layout
 * 
 * This is equivalent to removing all the elements where check_squeeze
 * returns true. Removed axes are moved to the end of the range. The new
 * end of the range is returned. 
 * 
 * @see check_squeeze
 * @see std::remove_if
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return ForwardIt Iterator to the new end of the range
 */
template <typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 
ForwardIt squeeze_layout_inplace(ForwardIt first, ForwardIt last) noexcept;

/**
 * @brief Squeezes a layout out-of-place
 * 
 * This is equivalent to removing all the elements where check_squeeze
 * returns true. Removed axes are moved to the end of the range. The new
 * end of the range is returned. 
 * 
 * @see check_squeeze
 * @see std::remove_if_copy
 * 
 * @tparam InputIt Input iterator
 * @tparam OutputIt Output iterator
 * @param first_from Iterator to the first element in the input range
 * @param last_from Iterator to the past-the-end element in the input range
 * @param first_to Iterator to hte first element in the output range.
 * Must have (last-first) elements available
 * @return ForwardIt2 Iterator to the past-the-end element in the output range
 */
template <typename InputIt, typename OutputIt>
XMIPP4_CONSTEXPR_CPP20 OutputIt squeeze_layout(InputIt first_from, 
                                               InputIt last_from,
                                               OutputIt first_to );

/**
 * @brief Broadcast multiple layouts to match their shapes.
 * 
 * @note All layouts must have the same rank (number of axes).
 * When broadcasting different sized layouts, the short ones
 * should be padded with phantom axes of extent 1 at the beginning 
 * before calling this function.
 * @tparam ForwardIt Forward iterators.
 * @param rank Rank of the layouts.
 * @param firsts Iterators to the beginning of the layouts.
 * @return bool True if successful, false otherwise. 
 */
template <typename... ForwardIt>
XMIPP4_CONSTEXPR_CPP20 bool broadcast_layouts(std::size_t rank,
                                              ForwardIt... firsts);

} // namespace multidimensional
} // namespace xmipp4

#include "strided_layout_utils.inl"
