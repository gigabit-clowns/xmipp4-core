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
 * @file test_layout_reference.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for multidimensional/layout_reference.hpp
 * @date 2025-07-11
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <xmipp4/core/multidimensional/layout_reference.hpp>

#include "mock/mock_layout.hpp"

using namespace xmipp4::multidimensional;

TEST_CASE("get_rank on an empty layout_reference should return 0")
{
    layout_reference<mock_layout> test_layout;
    REQUIRE( test_layout.get_rank() == 0 );
}

TEST_CASE("get_rank on an non-empty layout_reference should forward the underlying rank")
{
    mock_layout mock;

    const std::size_t rank = 65732;
    REQUIRE_CALL(mock, get_rank()).RETURN(rank);

    layout_reference<mock_layout> test_layout(std::move(mock));
    REQUIRE( test_layout.get_rank() == rank );
}

