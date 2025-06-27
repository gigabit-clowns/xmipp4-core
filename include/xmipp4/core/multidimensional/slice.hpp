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

#include "../platform/constexpr.hpp"
#include "../platform/attributes.hpp"

#include <ostream>
#include <type_traits>
#include <limits>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Class representing an slice of an array.
 *
 * This class is used to represent a slice of an array axis. It consists of
 * a start index, a count of elements and a step between elements.
 *
 * @tparam Start Type of the starting index. 
 * May be an integral, std::integral_constant or begin_tag.
 * @tparam Count Type of the count value
 * May be an integral, std::integral_constant or end_tag.
 * @tparam Step Type of the step. 
 * May be an integral, std::integral_constant or adjacent_tag.
 * 
 */
template <typename Start, typename Count, typename Step>
class slice
{
public:
    using start_type = Start;
    using count_type = Count;
    using step_type = Step;

    /**
     * @brief Constructor
     * 
     * @param start Index of the first element
     * @param count Number of elements.
     * @param step Step between consecutive elements
     */
    XMIPP4_CONSTEXPR slice(start_type start,
                           count_type count,
                           step_type step ) noexcept;

    /**
     * @brief Constructor with type conversion
     * 
     * @tparam Start2 
     * @tparam Step2 
     * @tparam Count2 
     * @param other The object to be copied
     */
    template <typename Start2, typename Count2, typename Step2>
    XMIPP4_CONSTEXPR slice(Start2 start,
                           Count2 count,
                           Step2 step ) noexcept;

    /**
     * @brief Copy constructor from other specialization of slice
     * 
     * @tparam Start2 
     * @tparam Step2 
     * @tparam Count2 
     * @param other The object to be copied
     */
    template <typename Start2, typename Count2, typename Step2>
    XMIPP4_CONSTEXPR slice(const slice<Start2, Count2, Step2>& other) noexcept;
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
     * @brief Set the index element count.
     * 
     * @param count New number of elements referred by this slice.
     */
    XMIPP4_CONSTEXPR void set_count(count_type count) noexcept;

    /**
     * @brief Get the element count.
     * 
     * @return count_type const& Number of elements referred by this slice.
     */
    XMIPP4_CONSTEXPR const count_type& get_count() const noexcept;

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
    XMIPP4_NO_UNIQUE_ADDRESS count_type m_count;
    XMIPP4_NO_UNIQUE_ADDRESS step_type m_step;

};





template <typename Start1, typename Count1, typename Step1, 
          typename Start2, typename Count2, typename Step2>
XMIPP4_CONSTEXPR bool
operator==( const slice<Start1, Count1, Step1>& lhs, 
            const slice<Start2, Count2, Step2>& rhs ) noexcept;

template <typename Start1, typename Count1, typename Step1, 
          typename Start2, typename Count2, typename Step2>
XMIPP4_CONSTEXPR bool
operator!=( const slice<Start1, Count1, Step1>& lhs, 
            const slice<Start2, Count2, Step2>& rhs ) noexcept;

template <typename Start, typename Count, typename Step>
std::ostream& operator<<(std::ostream& os, const slice<Start, Count, Step> &s);



template <typename T>
struct is_slice
    : std::false_type
{
};

template <typename Start, typename Count, typename Step>
struct is_slice<slice<Start, Count, Step>>
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
 * @brief Tag defining an undefined count value.
 * 
 */
struct end_tag {
    template <typename I, typename = typename std::enable_if<std::is_integral<I>::value>::type>
    XMIPP4_CONSTEXPR
    operator I() const noexcept { return std::numeric_limits<I>::max(); }
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
 * @brief Specialization of slice where all its components are dynamic.
 * 
 */
using dynamic_slice = slice<std::ptrdiff_t, std::size_t, std::ptrdiff_t>;

/**
 * @brief Special case of slice representing all elements
 * of an axis
 * 
 */
using all_slice = slice<begin_tag, end_tag, adjacent_tag>;

/**
 * @brief Construct an all_slice
 * 
 * @return An all_slice
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
 * @brief Construct an even_slice
 * 
 * @return An even_slice
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
 * @brief Construct an odd_slice
 * 
 * @return An odd_slice
 */
XMIPP4_CONSTEXPR odd_slice odd() noexcept;



/**
 * @brief Obtain a slice bounded at the end
 * 
 * @tparam Count 
 * @param count Number of elements.
 * @return slice<begin_tag, Count, adjacent_tag>
 */
template <typename Count>
XMIPP4_CONSTEXPR 
slice<begin_tag, Count, adjacent_tag> make_slice(Count count) noexcept;

/**
 * @brief Obtain a slice bounded at the beginning and the end
 * 
 * @tparam Start 
 * @tparam Count 
 * @param start Index of the first element
 * @param count Number of elements.
 * @return slice<Start, Count, adjacent_tag>
 */
template <typename Start, typename Count>
XMIPP4_CONSTEXPR 
slice<Start, Count, adjacent_tag> make_slice(Start start, Count count) noexcept;

/**
 * @brief Create a slice object
 * 
 * @tparam Start 
 * @tparam Step 
 * @tparam Count 
 * @param start Index of the first element.
 * @param count Number of elements.
 * @param step Step between adjacent element.
 * @return slice<Start, Count, Step>
 */
template <typename Start, typename Count, typename Step>
XMIPP4_CONSTEXPR 
slice<Start, Count, Step> make_slice(Start start, Count count, Step step) noexcept;

} // namespace multidimensional
} // namespace xmipp4

#include "slice.inl"
