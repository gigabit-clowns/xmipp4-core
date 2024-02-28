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

namespace xmipp4 
{

/**
 * @brief Class representing an slice of an array
 * 
 * @tparam Start Type of the starting index. May be an std::integral_constant or begin_tag
 * @tparam Step Type of the step. May be an std::integral_constant or adjacent_tag
 * @tparam Stop Type of the stopping index. May be an std::integral_constant or end_tag
 */
template <typename Start, typename Step, typename Stop>
class slice
{
public:
    using start_type = Start;
    using step_type = Step;
    using stop_type = Stop;

    /**
     * @brief Constructor
     * 
     * @param start Index of the first element
     * @param step Step between consecutive elements
     * @param stop Index of the past-the-end element
     */
    XMIPP4_CONSTEXPR slice(start_type start, 
                           step_type step, 
                           stop_type stop ) noexcept;

    /**
     * @brief Copy constructor from other specialization of slice
     * 
     * @tparam Start2 
     * @tparam Step2 
     * @tparam Stop2 
     * @param other The object to be copied
     */
    template <typename Start2, typename Step2, typename Stop2>
    XMIPP4_CONSTEXPR slice(const slice<Start2, Step2, Stop2>& other) noexcept;
    slice(const slice& other) = default;
    slice(slice&& other) = default;
    ~slice() = default;
    
    slice& operator=(const slice& other) = default;
    slice& operator=(slice&& other) = default;

    XMIPP4_CONSTEXPR bool operator==(const slice& other) const noexcept;
    XMIPP4_CONSTEXPR bool operator!=(const slice& other) const noexcept;

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
    XMIPP4_NO_UNIQUE_ADDRESS step_type m_step;
    XMIPP4_NO_UNIQUE_ADDRESS stop_type m_stop;

};

/**
 * @brief Tag defining the beginning on an axis
 * 
 */
struct begin_tag {};

/**
 * @brief Constant representing the beginning on an axis
 * 
 */
inline XMIPP4_CONST_CONSTEXPR begin_tag begin;

/**
 * @brief Tag defining the end on an axis
 * 
 */
struct end_tag {};

/**
 * @brief Constant representing the end on an axis
 * 
 */
inline XMIPP4_CONST_CONSTEXPR end_tag end;

/**
 * @brief Tag defining unit stride
 * 
 */
struct adjacent_tag {};

/**
 * @brief Constant representing unit stride
 * 
 */
inline XMIPP4_CONST_CONSTEXPR adjacent_tag adjacent;

/**
 * @brief Obtain a slice bounded at the end
 * 
 * @tparam Stop 
 * @param stop Index of the past-the-end element
 * @return generic_slice<begin_t, unit_step_t, Stop> 
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
 * @return generic_slice<Start, unit_step_t, Stop> 
 */
template <typename Start, typename Stop>
XMIPP4_CONSTEXPR slice<Start, adjacent_tag, Stop> 
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
 * @return generic_slice<Start, Step, Stop> 
 */
template <typename Start, typename Step, typename Stop>
XMIPP4_CONSTEXPR slice<Start, Step, Stop> 
make_slice(Start start, Step step, Stop stop) noexcept;



template <typename Start, typename Step, typename Stop>
std::ostream& operator<<(std::ostream& os, const slice<Start, Step, Stop> &s);
std::ostream& operator<<(std::ostream& os, begin_tag);
std::ostream& operator<<(std::ostream& os, end_tag);
std::ostream& operator<<(std::ostream& os, adjacent_tag);

} // namespace xmipp4

#include "slice.inl"
