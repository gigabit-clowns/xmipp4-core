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
 * @file test_checks.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for checks.hpp
 * @date 2024-11-12
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <xmipp4/core/compute/checks.hpp>

using namespace xmipp4::compute;

TEST_CASE( "require same type", "[checks]" )
{
    numerical_type result;
    REQUIRE_NOTHROW(result = require_same_type(numerical_type::int16, numerical_type::int16) );
    REQUIRE( result == numerical_type::int16 );
}

TEST_CASE( "require same type throws", "[checks]" )
{
    numerical_type result;
    REQUIRE_THROWS_AS( require_same_type(numerical_type::int16, numerical_type::int32), std::invalid_argument );
    REQUIRE_THROWS_WITH( require_same_type(numerical_type::int16, numerical_type::int32), "All buffers must have the same numerical type" );
}

TEST_CASE( "require same count", "[checks]" )
{
    std::size_t result;
    REQUIRE_NOTHROW(result = require_same_count(2, 2) );
    REQUIRE( result == 2 );
}

TEST_CASE( "require same count throws", "[checks]" )
{
    numerical_type result;
    REQUIRE_THROWS_AS( require_same_count(2, 3), std::invalid_argument );
    REQUIRE_THROWS_WITH( require_same_count(2, 3), "All buffers must have the same element count" );
}

TEST_CASE( "require valid source region", "[checks]" )
{
    const copy_region region(10, 15, 10);
    REQUIRE_NOTHROW( require_valid_source_region(region, 20) );
}

TEST_CASE( "require valid source region throws", "[checks]" )
{
    const copy_region region(10, 15, 10);
    REQUIRE_THROWS_AS( require_valid_source_region(region, 19), std::invalid_argument );
    REQUIRE_THROWS_WITH( require_valid_source_region(region, 19), "Source region must be within buffer bounds" );
}

TEST_CASE( "require valid destination region", "[checks]" )
{
    const copy_region region(15, 10, 10);
    REQUIRE_NOTHROW( require_valid_destination_region(region, 20) );
}

TEST_CASE( "require valid destination region throws", "[checks]" )
{
    const copy_region region(15, 10, 10);
    REQUIRE_THROWS_AS( require_valid_destination_region(region, 19), std::invalid_argument );
    REQUIRE_THROWS_WITH( require_valid_destination_region(region, 19), "Destination region must be within buffer bounds" );
}
