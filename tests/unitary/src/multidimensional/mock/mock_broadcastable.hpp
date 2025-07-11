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
 * @file mock_broadcastable.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Definition of a broadcastable class.
 * @date 2025-07-03
 */

#include <trompeloeil.hpp>

#include <xmipp4/core/span.hpp>

#include <vector>

namespace xmipp4
{
namespace multidimensional
{

class mock_broadcastable
{
public:
    static constexpr bool trompeloeil_movable_mock = true;

    MAKE_MOCK1(broadcast_extents_to_layout, void(std::vector<std::size_t>&), const);
    MAKE_MOCK1(broadcast_layout_to_extents, mock_broadcastable(span<const std::size_t>), const);

};

} // namespace multidimensional
} // namespace xmipp4
