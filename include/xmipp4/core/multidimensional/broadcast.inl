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
 * @file broadcast.inl
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation broadcast.hpp
 * @date 2025-07-03
 * 
 */

#include "broadcast.hpp"

#include <algorithm>
#include <functional>

namespace xmipp4 
{
namespace multidimensional
{

template <typename... Broadcastables>
inline
std::tuple<Broadcastables...> broadcast(std::vector<std::size_t> &extents,
                                       const Broadcastables&... items )
{
    (items.broadcast_dry(extents), ...); // TODO avoid using fold expressions
    return std::tuple<Broadcastables...>(items.broadcast_to(extents)...);
}


} // namespace multidimensional
} // namespace xmipp4
