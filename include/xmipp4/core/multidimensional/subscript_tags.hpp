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
 * @file subscript_tags.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines subscript tags
 * @date 2025-02-03
 * 
 */

#include "slice.hpp"
#include "index.hpp"
#include "../platform/constexpr.hpp"

#include <type_traits>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Tag to express the consumption of an arbitrary amount 
 * of axes until no slack is left on the right-hand dimensions.
 * NumPy equivalent of Ellipsis or "...".
 * 
 */
struct ellipsis_tag {};

XMIPP4_CONSTEXPR
bool operator==(ellipsis_tag lhs, ellipsis_tag rhs) noexcept;
XMIPP4_CONSTEXPR
bool operator!=(ellipsis_tag lhs, ellipsis_tag rhs) noexcept;

/**
 * @brief Construct a ellipsis_tag.
 * 
 * @return ellipsis_tag Newly constructed tag.
 */
XMIPP4_CONSTEXPR
ellipsis_tag ellipsis() noexcept;



/**
 * @brief Tag to express the creation a new axis of size one.
 * NumPy equivalent of numpy.newaxis() or "None".
 * 
 */
struct new_axis_tag {};

XMIPP4_CONSTEXPR
bool operator==(new_axis_tag, new_axis_tag) noexcept;
XMIPP4_CONSTEXPR
bool operator!=(new_axis_tag, new_axis_tag) noexcept;

XMIPP4_CONSTEXPR
new_axis_tag new_axis() noexcept;



template <typename T>
class subscript_traits;

template <>
class subscript_traits<ellipsis_tag>
{

};

template <>
class subscript_traits<new_axis_tag>
{

};

} // namespace multidimensional
} // namespace xmipp4

#include "subscript_tags.inl"
