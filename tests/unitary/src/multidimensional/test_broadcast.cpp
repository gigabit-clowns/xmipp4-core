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

#include "mock/mock_broadcastable.hpp"

using namespace xmipp4::multidimensional;

TEST_CASE("broadcast should call broadcast_dry and broadcast_to in order")
{
    trompeloeil::sequence seq;
    mock_broadcastable first;
    mock_broadcastable second;
    mock_broadcastable third;

    std::vector<std::size_t> extents;

    REQUIRE_CALL(first, broadcast_dry(std::ref(extents))).IN_SEQUENCE(seq);
    REQUIRE_CALL(second, broadcast_dry(std::ref(extents))).IN_SEQUENCE(seq);
    REQUIRE_CALL(third, broadcast_dry(std::ref(extents))).IN_SEQUENCE(seq);
    REQUIRE_CALL(first, broadcast_to(ANY(xmipp4::span<const std::size_t>))).RETURN(mock_broadcastable()).IN_SEQUENCE(seq);
    REQUIRE_CALL(second, broadcast_to(ANY(xmipp4::span<const std::size_t>))).RETURN(mock_broadcastable()).IN_SEQUENCE(seq);
    REQUIRE_CALL(third, broadcast_to(ANY(xmipp4::span<const std::size_t>))).RETURN(mock_broadcastable()).IN_SEQUENCE(seq);

    broadcast(extents, first, second, third);
}
