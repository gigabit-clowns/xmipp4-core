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

#include <type_traits>
#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Class to check if a type can be used as an index.
 * 
 * @tparam T Type to be checked.
 * 
 */
template <typename T, typename=void>
struct is_index
	: std::false_type
{
};

template <typename I>
struct is_index<I, typename std::enable_if<std::is_integral<I>::value>::type>
	: std::true_type
{
};

template <typename I, I value>
struct is_index<std::integral_constant<I, value>>
	: std::true_type
{
};



/**
 * @brief Ensure that the index is within bounds for a given extent.
 * 
 * This function ensures that the input index is within bounds for the
 * provided axis extent. In addition, it replaces negative values with
 * indices referred from the end of the array.
 * 
 * @tparam T Integral type or std::integral_constant.
 * @param index Index to be checked.
 * @param extent Extent of the indexed axis.
 * @return std::size_t Sanitize value. Guaranteed to be in [0, extent)
 * @throws std::out_of_range if the provided index is out of [-extent, extent)
 */
template <typename T>
typename std::enable_if<is_index<T>::value, std::size_t>::type
sanitize_index(T index, std::size_t extent);

} // namespace multidimensional
} // namespace xmipp4

#include "index.inl"
