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
#include "subscript_sequence.hpp"
#include "../platform/constexpr.hpp"

#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

struct column_major_tag {};

/**
 * @brief Construct a column major tag
 * 
 * @return column_major_tag
 */
XMIPP4_CONSTEXPR column_major_tag column_major() noexcept;


struct row_major_tag {};

/**
 * @brief Construct a row major tag
 * 
 * @return row_major_tag
 */
XMIPP4_CONSTEXPR row_major_tag row_major() noexcept;



/**
 * @brief Check if a pair of axes is column major.
 * 
 * Axes are considered to be in column major order if the
 * first axis is more contiguous in memory than the second one,
 * this is, if its stride is smaller in magnitude.
 * 
 * @param prev First axis
 * @param next Second axis
 * @return bool True if axes are in column major, False otherwise
 */
XMIPP4_CONSTEXPR bool check_axis_order(const axis_descriptor &prev, 
                                       const axis_descriptor &next,
                                       column_major_tag ) noexcept;

/**
 * @brief Check if a pair of axes is row major.
 * 
 * Axes are considered to be in row major order if the second
 * axis is more contiguous in memory than the first one,
 * this is, if its stride is smaller in magnitude.
 * 
 * @param prev First axis
 * @param next Second axis
 * @return bool True if axes are in row major, False otherwise
 */
XMIPP4_CONSTEXPR bool check_axis_order(const axis_descriptor &prev, 
                                       const axis_descriptor &next,
                                       row_major_tag ) noexcept;

/**
 * @brief Check if a pair of axes is overlapping
 * 
 * Axes are considered to be overlapping if the size 
 * of the major axis is strictly smaller than the stride
 * of the minor axis
 * 
 * @param prev First axis
 * @param next Second axis
 * @return bool True if axes are overlapping
 */
XMIPP4_CONSTEXPR bool check_axis_overlap(const axis_descriptor &major, 
                                         const axis_descriptor &minor ) noexcept;

/**
 * @brief Check if a pair of axes is regular
 * 
 * A pair of axes is regular if the minor axis (the slow one)
 * has a stride equal to the size of the major axis (the fast one)
 * 
 * @param major The axis with the smaller stride
 * @param minor The axis with the larger stride
 * @return bool True if axes are regular
 */
XMIPP4_CONSTEXPR bool is_regular(const axis_descriptor &major, 
                                 const axis_descriptor &minor ) noexcept;

/**
 * @brief Check if a sequence of axes has a particular ordering
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @param order OrderTag specifying axis ordering
 * @return True if all elements in the range are ordered
 */
template<typename ForwardIt, typename OrderTag>
XMIPP4_CONSTEXPR_CPP20 bool check_layout_order(ForwardIt first, 
                                               ForwardIt last,
                                               OrderTag &&order ) noexcept;

/**
 * @brief Check if a column major layout contains overlapping axes
 *
 * @see check_axis_overlap 
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return bool True if the range contains at least one overlapping pair. 
 * False otherwise
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 bool check_layout_overlap(ForwardIt first, 
                                                 ForwardIt last,
                                                 column_major_tag ) noexcept;

/**
 * @brief Check if a row major layout contains overlapping axes
 *
 * @see check_axis_overlap 
 * 
 * @tparam BidirIt Bidirectional iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return bool True if the range contains at least one overlapping pair. 
 * False otherwise
 */
template<typename BidirIt>
XMIPP4_CONSTEXPR_CPP20 bool check_layout_overlap(BidirIt first, 
                                                 BidirIt last,
                                                 row_major_tag ) noexcept;

/**
 * @brief Check if a column major layout is regular
 * 
 * @see is_regular
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return bool True if all the adjacent axis pairs are regular. 
 * False otherwise
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 bool is_regular_layout(ForwardIt first, 
                                              ForwardIt last,
                                              column_major_tag ) noexcept;

/**
 * @brief Check if a row major layout is regular
 * 
 * @see is_regular
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return bool True if all the adjacent axis pairs are regular. 
 * False otherwise
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 bool is_regular_layout(ForwardIt first, 
                                              ForwardIt last,
                                              row_major_tag ) noexcept;

/**
 * @brief Check if a column major layout is contiguous
 * 
 * A layout is contiguous if it is regular and its major axis
 * has a unit stride
 * 
 * @see is_regular_layout
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return bool True if the layout is contiguous. False otherwise
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 bool is_contiguous_layout(ForwardIt first, 
                                                 ForwardIt last,
                                                 column_major_tag ) noexcept;

/**
 * @brief Check if a row major layout is contiguous
 * 
 * A layout is contiguous if it is regular and its major axis
 * has a unit stride
 * 
 * @see is_regular_layout
 * 
 * @tparam BidirIt Bidirectional iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return bool True if the layout is contiguous. False otherwise
 */
template<typename BidirIt>
XMIPP4_CONSTEXPR_CPP20 bool is_contiguous_layout(BidirIt first, 
                                                 BidirIt last,
                                                 row_major_tag ) noexcept;

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
XMIPP4_CONSTEXPR_CPP20 std::size_t compute_contiguous_axis_strides(ForwardIt first,
                                                                   ForwardIt last,
                                                                   column_major_tag) noexcept;

/**
 * @brief Fill the strides of a layout such that it is contiguous
 * in row major ordering
 * 
 * @tparam BidirIt Bidirectional iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return std::size_t The volume, this is, the product of all sizes
 */
template<typename BidirIt>
XMIPP4_CONSTEXPR_CPP20 std::size_t compute_contiguous_axis_strides(BidirIt first,
                                                                   BidirIt last,
                                                                   row_major_tag) noexcept;

/**
 * @brief Copy the sizes of a layout to another range
 * 
 * @tparam InputIt Input iterator to a axis_descriptor
 * @tparam OutputIt Output iterator to a std::size_t
 * @param first First element in the input range
 * @param last Past-the-end element in the input range
 * @param out First element in the output range. 
 * Must be incrementable (last-first) times
 * @return OutputIt Iterator to the past-the-end element in the output range
 */
template<typename InputIt, typename OutputIt>
XMIPP4_CONSTEXPR_CPP20 OutputIt fill_shape_from_axes(InputIt first, 
                                                     InputIt last,
                                                     OutputIt out );

/**
 * @brief Finds the axis with the largest stride magnitude.
 * 
 * The axis with the largest stride is considered as the minor axis
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return ForwardIt The axis with the largest stride magnitude
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 ForwardIt find_max_stride(ForwardIt first, 
                                                 ForwardIt last ) noexcept;

/**
 * @brief Finds the axis with the smallest stride magnitude.
 * 
 * The axis with the smallest stride is considered as the major axis
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return ForwardIt The axis with the smallest stride magnitude
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 ForwardIt find_min_stride(ForwardIt first, 
                                                 ForwardIt last ) noexcept;

/**
 * @brief Reverses the ordering of a layout
 * 
 * @tparam BidirIt Bidirectional iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 */
template <typename BidirIt>
XMIPP4_CONSTEXPR_CPP20 void transpose_layout(BidirIt first, BidirIt last) noexcept;

/**
 * @brief Reverses the ordering of a layout out-of-place
 * 
 * @tparam BidirIt Bidirectional iterator
 * @tparam ForwardIt Forward iterator
 * @param first_from Iterator to the first element in the input range
 * @param last_from Iterator to the past-the-end element in the input range
 * @param first_to Iterator to hte first element in the output range.
 * Must have (last-first) elements available
 */
template <typename BidirIt, typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 ForwardIt transpose_layout(BidirIt first_from, BidirIt last_from,
                                                  ForwardIt first_to );

/**
 * @brief Computes the volume of a layout
 * 
 * This is equivalent to the product of all its axis sizes
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return std::size_t The volume of the layout
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 std::size_t compute_layout_volume(ForwardIt first,
                                                         ForwardIt last ) noexcept;

/**
 * @brief Flattens a regular column major layout
 * 
 * @tparam ForwardIt Forward iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return axis_descriptor Axis containing all the flattened axes
 */
template<typename ForwardIt>
XMIPP4_CONSTEXPR_CPP20 axis_descriptor flatten_regular_layout(ForwardIt first,
                                                              ForwardIt last,
                                                              column_major_tag ) noexcept;

/**
 * @brief Flattens a regular row major layout
 * 
 * @tparam BidirIt Bidirectional iterator
 * @param first Iterator to the first element in the range
 * @param last Iterator to the past-the-end element in the range
 * @return axis_descriptor Axis containing all the flattened axes
 */
template<typename BidirIt>
XMIPP4_CONSTEXPR_CPP20 axis_descriptor flatten_regular_layout(BidirIt first,
                                                              BidirIt last,
                                                              row_major_tag ) noexcept;

/**
 * @brief Check if an axis can be squeezed
 * 
 * An axis can be squeezed only if it has a extent of 1
 * 
 * @param axis The axis to be checked
 * @return bool True if the axis can be squeezed. False otherwise
 */
XMIPP4_CONSTEXPR bool check_squeeze(const axis_descriptor &axis) noexcept;

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
XMIPP4_CONSTEXPR_CPP20 ForwardIt squeeze_layout(ForwardIt first, ForwardIt last) noexcept;

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
 * @tparam ForwardIt1 Forward iterator
 * @tparam ForwardIt2 Forward iterator
 * @param first_from Iterator to the first element in the input range
 * @param last_from Iterator to the past-the-end element in the input range
 * @param first_to Iterator to hte first element in the output range.
 * Must have (last-first) elements available
 * @return ForwardIt2 Iterator to the past-the-end element in the output range
 */
template <typename ForwardIt1, typename ForwardIt2>
XMIPP4_CONSTEXPR_CPP20 ForwardIt2 squeeze_layout(ForwardIt1 first_from, 
                                                 ForwardIt1 last_from,
                                                 ForwardIt2 first_to );


template<typename InputIt, typename OutputIt, typename... Subscripts>
OutputIt apply_subscripts_to_layout(InputIt first, 
                                    InputIt last,
                                    OutputIt out,
                                    const subscript_sequence<Subscripts...>& subscripts,
                                    std::ptrdiff_t &offset );
                                                 

} // namespace multidimensional
} // namespace xmipp4

#include "strided_layout_utils.inl"
