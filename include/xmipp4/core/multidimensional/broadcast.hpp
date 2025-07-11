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
 * @file broadcast.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines broadcasting utilities
 * @date 2025-07-02
 * 
 */

#include <vector>
#include <tuple>

namespace xmipp4 
{
namespace multidimensional
{

/**
 * @brief Function to broadcast multiple layouts/arrays into a common
 * shape.
 * 
 * @tparam Broadcastables Broadastable types. These types must implement
 * broadcast_extents_to_layout and broadcast_layout_to_extents methods.
 * @param extents Input/output extents. The contents of extents participate
 * in the broadcasting operation. Leave it empty to avoid this. The resulting
 * consensus shape will be stored in this variable.
 * @param items Items to be broadcasted.
 * @return std::tuple<Broadcastables...> Broadcasted items. When successful,
 * all of the returned items will have the same shape as the values
 * written in extents.
 */
template <typename... Broadcastables>
std::tuple<Broadcastables...> broadcast(std::vector<std::size_t> &extents,
                                        const Broadcastables&... items );

} // namespace multidimensional
} // namespace xmipp4

#include "broadcast.inl"
