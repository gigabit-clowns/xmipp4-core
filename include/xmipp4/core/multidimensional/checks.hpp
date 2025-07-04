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
 * @file checks.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines validation functions
 * @date 2025-02-06
 * 
 */

#include <cstddef>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Check that the permutation is valid for a given layout.
 * 
 * A permutation is valid if it is a permutation of the [0, count) range.
 * 
 * @tparam ForwardIt Forward iterator to std::size_t-s.
 * @param first Iterator to the first element in the range.
 * @param last Iterator to the past-the-end element in the range.
 * @param count Number of axes in the layout.
 * @throws std::invalid_argument If the permutation is not valid.
 */
template <typename ForwardIt>
void check_axis_permutation(ForwardIt first, ForwardIt last, std::size_t count);

} // namespace multidimensional
} // namespace xmipp4

#include "checks.inl"
