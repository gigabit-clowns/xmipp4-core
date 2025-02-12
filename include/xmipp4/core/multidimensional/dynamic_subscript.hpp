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
 * @file dynamic_subscript.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines dynamic_subscript class
 * @date 2025-02-03
 * 
 */

#include <cstddef>

#include "subscript_tags.hpp"
#include "slice.hpp"
#include "../platform/attributes.hpp"

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Class describing a runtime defined subscript.
 * 
 * A subscript may be either a
 * - ellipsis_tag
 * - new_axis_tag
 * - index (std::ptrdiff_t)
 * - slice (dynamic_slice)
 * 
 */
class dynamic_subscript
{
public:
    /**
     * @brief Enumeration holding the values that can hold this object. 
     * 
     */
    enum class subscript_type
    {
        ellipsis,
        new_axis,
        index,
        slice
    };

    /**
     * @brief Construct a dynamic_subscript holding an ellipsis_tag.
     * 
     */
    XMIPP4_CONSTEXPR
    dynamic_subscript(ellipsis_tag) noexcept;

    /**
     * @brief Construct a dynamic_subscript holding a new_axis_tag.
     * 
     */
    XMIPP4_CONSTEXPR
    dynamic_subscript(new_axis_tag) noexcept;

    /**
     * @brief Construct a dynamic_subscript holding an index.
     * 
     * @tparam I Index type. 
     * @param index The index that is assigned to this object.

     */
    template <typename I, typename = typename std::enable_if<is_index<I>::value::type>>
    XMIPP4_CONSTEXPR
    dynamic_subscript(I index) noexcept;

    /**
     * @brief Construct a dynamic_subscript holding a slice.
     * 
     * @tparam Start Type of the start value.
     * @tparam Stop Type of the stop value.
     * @tparam Step Type of the step value.
     * @param slice the slice that is assigned to this object.
     * 
     */
    template <typename Start, typename Stop, typename Step>
    XMIPP4_CONSTEXPR
    dynamic_subscript(const slice<Start, Stop, Step> &slice) noexcept;

    dynamic_subscript(const dynamic_subscript &other) = default;
    dynamic_subscript(dynamic_subscript &&other) = default;
    ~dynamic_subscript() = default;

    dynamic_subscript& operator=(const dynamic_subscript &other) = default;
    dynamic_subscript& operator=(dynamic_subscript &&other) = default;

    /**
     * @brief Get the subscript type held by this object.
     * 
     * @return subscript_type 
     */
    XMIPP4_CONSTEXPR
    subscript_type get_subscript_type() const noexcept;

    /**
     * @brief Get the index.
     * 
     * This method shall only be called if get_subscript_type() returns
     * subscript_type::index. Otherwise behavior is undefined.
     * 
     * @return std::ptrdiff_t The index held by this object.
     */
    XMIPP4_CONSTEXPR
    std::ptrdiff_t get_index() const noexcept;

    /**
     * @brief Get the slice.
     * 
     * This method shall only be called if get_subscript_type() returns
     * subscript_type::slice. Otherwise behavior is undefined.
     * 
     * @return const dynamic_slice& The slice held by this object.
     */
    XMIPP4_CONSTEXPR
    const dynamic_slice& get_slice() const noexcept;

private:
    dynamic_slice m_data; // When holding index, start value is used.
    subscript_type m_type;

};

} // namespace multidimensional
} // namespace xmipp4

#include "dynamic_subscript.inl"
