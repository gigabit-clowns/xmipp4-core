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
 * @file test_device_index.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for device_index.hpp
 * @date 2024-10-29
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/compute/device_index.hpp>

#include <sstream>


using namespace xmipp4::compute;

TEST_CASE( "construct device index", "[device_index]" )
{
    device_index index("example-backend", 4);
    REQUIRE( index.get_backend_name() == "example-backend" );
    REQUIRE( index.get_device_id() == 4 );
}

TEST_CASE( "parse device index", "[device_index]" )
{
    device_index index;

    REQUIRE( parse_device_index("cuda:3", index) );
    REQUIRE( index.get_backend_name() == "cuda" );
    REQUIRE( index.get_device_id() == 3 );

    REQUIRE( parse_device_index("cuda", index) );
    REQUIRE( index.get_backend_name() == "cuda" );
    REQUIRE( index.get_device_id() == 0 );
}

TEST_CASE( "parse invalid device index", "[device_index]" )
{
    device_index index;
    REQUIRE( !parse_device_index("3:example", index) );
    REQUIRE( !parse_device_index(":3", index) );
    REQUIRE( !parse_device_index("test:", index) );
    REQUIRE( !parse_device_index(":", index) );
    REQUIRE( !parse_device_index("", index) );
}

TEST_CASE( "device index to ostream", "[device_index]" )
{
    std::ostringstream oss;
    const device_index index("example", 4);
    oss << index;
    REQUIRE( oss.str() == "example:4" );
}
