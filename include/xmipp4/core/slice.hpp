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
 * @file slice.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of slice class and helper tags
 * @date 2023-08-13
 * 
 */

#include "platform/constexpr.hpp"
#include "platform/attributes.hpp"

#include <ostream>
#include <type_traits>
#include <limits>

namespace xmipp4 
{

/**
 * @brief Class representing an slice of an array.
 *
 * The slice class holds two indices representing the range of referenced 
 * items. Similarly to STL iterators, these indices refer to the first and 
 * past-the-end elements of the range. In addition, a step value is also 
 * stored, which defines the stride (in elements) when accessing contiguous
 * items. This step cannot be zero. Negative values are allowed. When using
 * a negative step, the slice refers to (start, stop] range, where 
 * start >= stop (similarly to reverse iterators in the STL library). 
 * When using a positive step, the slice refers to the [start, stop) range, 
 * where start <= stop.
 *  
 * @tparam Start Type of the starting index. 
 * May be an integral, std::integral_constant, begin_tag or end_tag.
 * @tparam Stop Type of the stopping index.
 * May be an integral, std::integral_constant, begin_tag or end_tag.
 * @tparam Step Type of the step. 
 * May be an integral, std::integral_constant or adjacent_tag.
 * 
 */
template <typename Start, typename Stop, typename Step>
class slice
{
public:
    using start_type = Start;
    using stop_type = Stop;
    using step_type = Step;

    /**
     * @brief Constructor
     * 
     * @param start Index of the first element
     * @param stop Index of the past-the-end element
     * @param step Step between consecutive elements
     */
    XMIPP4_CONSTEXPR slice(start_type start,
                           stop_type stop,
                           step_type step ) noexcept;

    /**
     * @brief Constructor with type conversion
     * 
     * @tparam Start2 
     * @tparam Step2 
     * @tparam Stop2 
     * @param other The object to be copied
     */
    template <typename Start2, typename Stop2, typename Step2>
    XMIPP4_CONSTEXPR slice(Start2 start,
                           Stop2 stop,
                           Step2 step ) noexcept;

    /**
     * @brief Copy constructor from other specialization of slice
     * 
     * @tparam Start2 
     * @tparam Step2 
     * @tparam Stop2 
     * @param other The object to be copied
     */
    template <typename Start2, typename Stop2, typename Step2>
    XMIPP4_CONSTEXPR slice(const slice<Start2, Stop2, Step2>& other) noexcept;
    slice() = default;
    slice(const slice& other) = default;
    slice(slice&& other) = default;
    ~slice() = default;
    
    slice& operator=(const slice& other) = default;
    slice& operator=(slice&& other) = default;

    /**
     * @brief Set the starting index
     * 
     * @param start Index of the first element
     */
    XMIPP4_CONSTEXPR void set_start(start_type start) noexcept;

    /**
     * @brief Get the starting index
     * 
     * @return start_type const& Index of the first element
     */
    XMIPP4_CONSTEXPR const start_type& get_start() const noexcept;
    
    /**
     * @brief Set the stopping index
     * 
     * @param stop Index of the past-the-end element
     */
    XMIPP4_CONSTEXPR void set_stop(stop_type stop) noexcept;

    /**
     * @brief Get the stopping index
     * 
     * @return stop_type const& Index of the past-the-end element
     */
    XMIPP4_CONSTEXPR const stop_type& get_stop() const noexcept;

    /**
     * @brief Set the step
     * 
     * @param step Step between consecutive elements
     */
    XMIPP4_CONSTEXPR void set_step(step_type step) noexcept;

    /**
     * @brief Get the step
     * 
     * @return step_type const& Step between consecutive elements
     */
    XMIPP4_CONSTEXPR const step_type& get_step() const noexcept;

private:
    // Use non-unique addresses as they
    // may be made of an empty type such
    // as std::integral_constant
    XMIPP4_NO_UNIQUE_ADDRESS start_type m_start;
    XMIPP4_NO_UNIQUE_ADDRESS stop_type m_stop;
    XMIPP4_NO_UNIQUE_ADDRESS step_type m_step;

};

template <typename Start1, typename Stop1, typename Step1, 
          typename Start2, typename Stop2, typename Step2>
XMIPP4_CONSTEXPR bool
operator==( const slice<Start1, Stop1, Step1>& lhs, 
            const slice<Start2, Stop2, Step2>& rhs ) noexcept;

template <typename Start1, typename Stop1, typename Step1, 
          typename Start2, typename Stop2, typename Step2>
XMIPP4_CONSTEXPR bool
operator!=( const slice<Start1, Stop1, Step1>& lhs, 
            const slice<Start2, Stop2, Step2>& rhs ) noexcept;

template <typename Start, typename Stop, typename Step>
std::ostream& operator<<(std::ostream& os, const slice<Start, Stop, Step> &s);



template <typename T, typename=void>
struct is_slice
    : std::false_type
{
};

template <typename Start, typename Stop, typename Step>
struct is_slice<slice<Start, Stop, Step>>
    : std::true_type
{
};



/**
 * @brief Tag defining the beginning on an axis
 * 
 */
struct begin_tag {
    template <typename I, typename = typename std::enable_if<std::is_integral<I>::value>::type>
    XMIPP4_CONSTEXPR
    operator I() const noexcept { return 0; }
};

/**
 * @brief Construct a begin_tag
 * 
 * @return A begin_tag
 */
XMIPP4_CONSTEXPR begin_tag begin() noexcept;

XMIPP4_CONSTEXPR bool
operator==(const begin_tag& lhs, const begin_tag& rhs) noexcept;

XMIPP4_CONSTEXPR bool
operator!=(const begin_tag& lhs, const begin_tag& rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(const begin_tag& lhs, I rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(const begin_tag& lhs, I rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(I lhs, const begin_tag& rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(I lhs, const begin_tag& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, begin_tag);




/**
 * @brief Tag defining the end on an axis
 * 
 */
struct end_tag {
    template <typename I, typename = typename std::enable_if<std::is_integral<I>::value>::type>
    XMIPP4_CONSTEXPR
    operator I() const noexcept { return (std::numeric_limits<I>::max)(); }
};

/**
 * @brief Construct an end_tag
 * 
 * @return An end_tag
 */
XMIPP4_CONSTEXPR end_tag end() noexcept;

/**
 * @brief Cast types preserving special end value.
 * 
 * @tparam To Type to be casted to. Must be integral type.
 * @tparam From Type to be casted from. Must be integral type, 
 * end_tag or std::integral_constant.
 * @param x Value to be casted from.
 * @return To Casted value.
 */
template <typename To, typename From>
XMIPP4_NODISCARD XMIPP4_CONSTEXPR
To propagate_end(From x) noexcept;

XMIPP4_CONSTEXPR bool
operator==(const end_tag& lhs, const end_tag& rhs) noexcept;

XMIPP4_CONSTEXPR bool
operator!=(const end_tag& lhs, const end_tag& rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(const end_tag& lhs, I rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(const end_tag& lhs, I rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(I lhs, const end_tag& rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(I lhs, const end_tag& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, end_tag);



/**
 * @brief Tag defining unit step
 * 
 */
struct adjacent_tag {
    template <typename I, typename = typename std::enable_if<std::is_integral<I>::value>::type>
    XMIPP4_CONSTEXPR
    operator I() const noexcept { return 1; }
};

/**
 * @brief Construct an adjacent_tag
 * 
 * @return An adjacent_tag
 */
XMIPP4_CONSTEXPR adjacent_tag adjacent() noexcept;

XMIPP4_CONSTEXPR bool
operator==(const adjacent_tag& lhs, const adjacent_tag& rhs) noexcept;

XMIPP4_CONSTEXPR bool
operator!=(const adjacent_tag& lhs, const adjacent_tag& rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(const adjacent_tag& lhs, I rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(const adjacent_tag& lhs, I rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(I lhs, const adjacent_tag& rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(I lhs, const adjacent_tag& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, adjacent_tag);



/**
 * @brief Tag defining to transverse the axis in reversed
 * ordering with unit step.
 * 
 */
struct reversed_tag {
    template <typename I, typename = typename std::enable_if<std::is_integral<I>::value &&
                                                             std::is_signed<I>::value >::type >
    XMIPP4_CONSTEXPR
    operator I() const noexcept { return -1; }
};

/**
 * @brief Construct an reversed_tag
 * 
 * @return An reversed_tag
 */
XMIPP4_CONSTEXPR reversed_tag reversed() noexcept;

XMIPP4_CONSTEXPR bool
operator==(const reversed_tag& lhs, const reversed_tag& rhs) noexcept;

XMIPP4_CONSTEXPR bool
operator!=(const reversed_tag& lhs, const reversed_tag& rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(const reversed_tag& lhs, I rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(const reversed_tag& lhs, I rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator==(I lhs, const reversed_tag& rhs) noexcept;

template <typename I>
XMIPP4_CONSTEXPR 
typename std::enable_if<std::is_integral<I>::value, bool>::type
operator!=(I lhs, const reversed_tag& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, reversed_tag);



/**
 * @brief Special case of slice representing all elements
 * of an axis
 * 
 */
using all_slice = slice<begin_tag, end_tag, adjacent_tag>;

/**
 * @brief Construct an all_tag
 * 
 * @return An all_tag
 */
XMIPP4_CONSTEXPR all_slice all() noexcept;



/**
 * @brief Special case of slice representing even elements
 * of an axis
 * 
 */
using even_slice = slice<std::integral_constant<std::size_t, 0>, 
                         end_tag,
                         std::integral_constant<std::size_t, 2> >;

/**
 * @brief Construct an even_tag
 * 
 * @return An even_tag
 */
XMIPP4_CONSTEXPR even_slice even() noexcept;



/**
 * @brief Special case of slice representing odd elements
 * of an axis
 * 
 */
using odd_slice = slice<std::integral_constant<std::size_t, 1>, 
                        end_tag,
                        std::integral_constant<std::size_t, 2> >;

/**
 * @brief Construct an odd_tag
 * 
 * @return An odd_tag
 */
XMIPP4_CONSTEXPR odd_slice odd() noexcept;



/**
 * @brief Obtain a slice bounded at the end
 * 
 * @tparam Stop 
 * @param stop Index of the past-the-end element
 * @return slice<begin_tag, Stop, adjacent_tag>
 */
template <typename Stop>
XMIPP4_CONSTEXPR slice<begin_tag, Stop, adjacent_tag>
make_slice(Stop stop) noexcept;

/**
 * @brief Obtain a slice bounded at the beginning and the end
 * 
 * @tparam Start 
 * @tparam Stop 
 * @param start Index of the first element
 * @param stop Index of the past-the-end element
 * @return slice<Start, Stop, adjacent_tag>
 */
template <typename Start, typename Stop>
XMIPP4_CONSTEXPR slice<Start, Stop, adjacent_tag>
make_slice(Start start, Stop stop) noexcept;

/**
 * @brief Create a slice object
 * 
 * @tparam Start 
 * @tparam Step 
 * @tparam Stop 
 * @param start Index of the first element
 * @param step Step between adjacent element
 * @param stop Index of the past-the-end element
 * @return slice<Start, Stop, Step>
 */
template <typename Start, typename Stop, typename Step>
XMIPP4_CONSTEXPR slice<Start, Stop, Step>
make_slice(Start start, Stop stop, Step step) noexcept;

/**
 * @brief Validates an slice index (start or stop).
 * This function performs the following actions
 * - Replaces end value with size
 * - Replaces negative values with size + index
 * - Checks for bounds
 * 
 * @tparam T Type of the index.
 * @param index The index to be validated.
 * @param size Size of the referenced array.
 * @return std::size_t Index sanitized according to the array size.
 */
template <typename T>
std::size_t sanitize_slice_index(T index, std::size_t size);

/**
 * @brief Sanitize step and check coherency with start stop value
 * ordering.
 * 
 * @tparam T Type of the step.
 * @param step Step of the slice.
 * @param start Start value.
 * @param stop Stop value.
 * @return std::ptrdiff_t Sanitized step.
 */
template <typename T>
std::ptrdiff_t sanitize_slice_step(T step, std::size_t start, std::size_t stop);

/**
 * @brief Sanitizes and unpacks a slice for use.
 * 
 * @tparam Start Type of the start value.
 * @tparam Step Type of the step value.
 * @tparam Stop Type of the stop value.
 * @param slc The slice to be sanitized.
 * @param size Size of the referenced array.
 * @param start Output of start value.
 * @param stop Output of stop value.
 * @param step Output of step value.
 */
template <typename Start, typename Step, typename Stop>
void sanitize_slice(const slice<Start, Step, Stop> &slc,
                    std::size_t size,
                    std::size_t &start,
                    std::size_t &stop,
                    std::ptrdiff_t &step );

/**
 * @brief Compute the number of elements referenced by a slice.
 * Sanitized parameters must be provided. Otherwise behaviour is undefined.
 * 
 * @param start Start value.
 * @param stop Stop value.
 * @param step Step value.
 * @return std::size_t Number of elements referenced by the slice.
 */
XMIPP4_CONSTEXPR
std::size_t compute_slice_size(std::size_t start, 
                               std::size_t stop, 
                               std::ptrdiff_t step ) noexcept;

/**
 * @brief Compute the pivot index of the slice. Sanitized parameters
 * must be provided. Otherwise behaviour is undefined.
 * 
 * The pivot point is the element referred by the start index. When 
 * the step of the slice is positive, this is the start value itself. 
 * When the step is negative, this is the start value minus one, as 
 * this index is exclusive. However, if the start value is zero, it is 
 * not decremented (in this case, the slice has size zero).
 * 
 * @param start Start index of the slice.
 * @param step Step of the slice.
 * @return std::size_t The pivot index.
 */
XMIPP4_CONSTEXPR
std::size_t compute_slice_pivot(std::size_t start, 
                                std::ptrdiff_t step ) noexcept;

} // namespace xmipp4

#include "slice.inl"
