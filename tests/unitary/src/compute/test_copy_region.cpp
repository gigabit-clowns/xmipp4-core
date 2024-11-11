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
 * @file test_copy_region.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for copy_region.hpp
 * @date 2024-11-11
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/copy_region.hpp>

using namespace xmipp4::compute;

TEST_CASE( "construct copy region", "[copy_region]" )
{
    const copy_region region(4, 6, 10);
    REQUIRE( region.get_source_offset() == 4 );
    REQUIRE( region.get_destination_offset() == 6 );
    REQUIRE( region.get_count() == 10 );
}

TEST_CASE( "default construct copy region", "[copy_region]" )
{
    const copy_region region;
    REQUIRE( region.get_source_offset() == 0 );
    REQUIRE( region.get_destination_offset() == 0 );
    REQUIRE( region.get_count() == 0 );
}

TEST_CASE( "convert region to bytes", "[copy_region]" )
{
    const copy_region region(4, 6, 10);
    const auto region_bytes = as_bytes(region, 8U);
    REQUIRE( region.get_source_offset() == 32 );
    REQUIRE( region.get_destination_offset() == 48 );
    REQUIRE( region.get_count() == 80 );
}

