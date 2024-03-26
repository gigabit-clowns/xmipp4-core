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
 * @brief Class representing an slice of an array
 * 
 * @tparam Start Type of the starting index. May be an integral, std::integral_constant or begin_tag
 * @tparam Stride Type of the stride. May be an integral, std::integral_constant or adjacent_tag
 * @tparam Stop Type of the stopping index. May be an integral, std::integral_constant or end_tag
 */
template <typename Start, typename Stride, typename Stop>
class slice
{
public:
    using start_type = Start;
    using stride_type = Stride;
    using stop_type = Stop;

    /**
     * @brief Constructor
     * 
     * @param start Index of the first element
     * @param stride Stride between consecutive elements
     * @param stop Index of the past-the-end element
     */
    XMIPP4_CONSTEXPR slice(start_type start, 
                           stride_type stride, 
                           stop_type stop ) noexcept;

    /**
     * @brief Constructor with type conversion
     * 
     * @tparam Start2 
     * @tparam Stride2 
     * @tparam Stop2 
     * @param other The object to be copied
     */
    template <typename Start2, typename Stride2, typename Stop2>
    XMIPP4_CONSTEXPR slice(Start2 start,
                           Stride2 stride,
                           Stop2 stop ) noexcept;

    /**
     * @brief Copy constructor from other specialization of slice
     * 
     * @tparam Start2 
     * @tparam Stride2 
     * @tparam Stop2 
     * @param other The object to be copied
     */
    template <typename Start2, typename Stride2, typename Stop2>
    XMIPP4_CONSTEXPR slice(const slice<Start2, Stride2, Stop2>& other) noexcept;
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
     * @brief Set the stride
     * 
     * @param stride Stride between consecutive elements
     */
    XMIPP4_CONSTEXPR void set_stride(stride_type stride) noexcept;

    /**
     * @brief Get the stride
     * 
     * @return stride_type const& Stride between consecutive elements
     */
    XMIPP4_CONSTEXPR const stride_type& get_stride() const noexcept;
    
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

private:
    // Use non-unique addresses as they
    // may be made of an empty type such
    // as std::integral_constant
    XMIPP4_NO_UNIQUE_ADDRESS start_type m_start;
    XMIPP4_NO_UNIQUE_ADDRESS stride_type m_stride;
    XMIPP4_NO_UNIQUE_ADDRESS stop_type m_stop;

};

template <typename Start1, typename Stride1, typename Stop1, 
          typename Start2, typename Stride2, typename Stop2>
XMIPP4_CONSTEXPR bool
operator==( const slice<Start1, Stride1, Stop1>& lhs, 
            const slice<Start2, Stride2, Stop2>& rhs ) noexcept;

template <typename Start1, typename Stride1, typename Stop1, 
          typename Start2, typename Stride2, typename Stop2>
XMIPP4_CONSTEXPR bool
operator!=( const slice<Start1, Stride1, Stop1>& lhs, 
            const slice<Start2, Stride2, Stop2>& rhs ) noexcept;

template <typename Start, typename Stride, typename Stop>
std::ostream& operator<<(std::ostream& os, const slice<Start, Stride, Stop> &s);





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

std::ostream& operator<<(std::ostream& os, begin_tag);



/**
 * @brief Tag defining unit stride
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

std::ostream& operator<<(std::ostream& os, adjacent_tag);



/**
 * @brief Tag defining the end on an axis
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

XMIPP4_CONSTEXPR bool
operator==(const end_tag& lhs, const end_tag& rhs) noexcept;

XMIPP4_CONSTEXPR bool
operator!=(const end_tag& lhs, const end_tag& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, end_tag);




/**
 * @brief Special case of slice representing all elements+
 * of an axis
 * 
 */
struct all_tag : slice<begin_tag, adjacent_tag, end_tag>
{
};

/**
 * @brief Construct an all_tag
 * 
 * @return An all_tag
 */
XMIPP4_CONSTEXPR all_tag all() noexcept;

std::ostream& operator<<(std::ostream& os, all_tag);



/**
 * @brief Obtain a slice bounded at the end
 * 
 * @tparam Stop 
 * @param stop Index of the past-the-end element
 * @return generic_slice<begin_t, unit_stride_t, Stop> 
 */
template <typename Stop>
XMIPP4_CONSTEXPR slice<begin_tag, adjacent_tag, Stop> 
make_slice(Stop stop) noexcept;

/**
 * @brief Obtain a slice bounded at the beginning and the end
 * 
 * @tparam Start 
 * @tparam Stop 
 * @param start Index of the first element
 * @param stop Index of the past-the-end element
 * @return generic_slice<Start, unit_stride_t, Stop> 
 */
template <typename Start, typename Stop>
XMIPP4_CONSTEXPR slice<Start, adjacent_tag, Stop> 
make_slice(Start start, Stop stop) noexcept;

/**
 * @brief Create a slice object
 * 
 * @tparam Start 
 * @tparam Stride 
 * @tparam Stop 
 * @param start Index of the first element
 * @param stride Stride between adjacent element
 * @param stop Index of the past-the-end element
 * @return generic_slice<Start, Stride, Stop> 
 */
template <typename Start, typename Stride, typename Stop>
XMIPP4_CONSTEXPR slice<Start, Stride, Stop> 
make_slice(Start start, Stride stride, Stop stop) noexcept;

} // namespace xmipp4

#include "slice.inl"
