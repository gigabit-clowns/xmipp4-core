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
 * @file test_broadcast.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for multidimensional/broadcast.hpp
 * @date 2024-07-02
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <xmipp4/core/multidimensional/broadcast.hpp>
#include <xmipp4/core/platform/compiler.h>

#include "mock/mock_broadcastable.hpp"

using namespace xmipp4::multidimensional;

TEST_CASE("broadcast should call broadcast_extents_to_layout and broadcast_layout_to_extents in order")
{
    mock_broadcastable first;
    mock_broadcastable second;
    mock_broadcastable third;

    std::vector<std::size_t> extents;
    
    #if defined(XMIPP4_GCC) // FIXME 
        REQUIRE_CALL(first, broadcast_extents_to_layout(std::ref(extents)));
        REQUIRE_CALL(second, broadcast_extents_to_layout(std::ref(extents)));
        REQUIRE_CALL(third, broadcast_extents_to_layout(std::ref(extents)));
        REQUIRE_CALL(first, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable());
        REQUIRE_CALL(second, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable());
        REQUIRE_CALL(third, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .RETURN(mock_broadcastable());
    #else
        trompeloeil::sequence seq;

        REQUIRE_CALL(first, broadcast_extents_to_layout(std::ref(extents)))
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(second, broadcast_extents_to_layout(std::ref(extents)))
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(third, broadcast_extents_to_layout(std::ref(extents)))
            .IN_SEQUENCE(seq);
        REQUIRE_CALL(first, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .IN_SEQUENCE(seq)
            .RETURN(mock_broadcastable());
        REQUIRE_CALL(second, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .IN_SEQUENCE(seq)
            .RETURN(mock_broadcastable());
        REQUIRE_CALL(third, broadcast_layout_to_extents(ANY(xmipp4::span<const std::size_t>)))
            .IN_SEQUENCE(seq)
            .RETURN(mock_broadcastable());
    #endif

    broadcast(extents, first, second, third);
}
