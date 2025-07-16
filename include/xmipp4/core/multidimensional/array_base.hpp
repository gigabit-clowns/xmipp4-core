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
 * @file array_base.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines array_base class
 * @date 2025-07-15
 * 
 */

#include <cstddef>

#include "../platform/attributes.hpp"

namespace xmipp4 
{
namespace multidimensional
{

template <typename T>
class array_base
{
public:
    using self_type = T;

    array_base() = default;
    array_base(const array_base &other) = default;
    array_base(array_base&& other) = default;
    ~array_base() = default;

    array_base& operator=(const array_base &other) = default;
    array_base& operator=(array_base&& other) = default;

    /**
     * @brief Get the rank of the array_base.
     * 
     * The rank of the array_base is the number of dimensions. Shorthand for
     * get_layout().get_rank()
     * 
     * @return std::size_t The rank.
     * 
     */
    XMIPP4_NODISCARD
    std::size_t get_rank() const noexcept;

};

} // namespace multidimensional
} // namespace xmipp4

#include "array_base.inl"
