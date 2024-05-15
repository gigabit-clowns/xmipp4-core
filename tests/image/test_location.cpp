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
 * @file test_location.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for image/location.hpp
 * @date 2024-05-15
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/image/location.hpp>

#include <string>
#include <sstream>

using namespace xmipp4::image;

TEST_CASE("parse image location", "[image location]")
{
    SECTION("without position")
    {
        const std::string path = "/home/xmipp4/image.mrc";

        location loc;
        REQUIRE( parse_location(path, loc) );
        REQUIRE( loc.get_filename() == path );
        REQUIRE( loc.get_position() == location::no_position );
    }

    SECTION("with position")
    {
        const std::string path = "1234@/home/xmipp4/image.mrc";

        location loc;
        REQUIRE( parse_location(path, loc) );
        REQUIRE( loc.get_filename() == "/home/xmipp4/image.mrc" );
        REQUIRE( loc.get_position() == 1234 );
    }

    SECTION("with invalid position")
    {
        const std::string path = "1234a@/home/xmipp4/image.mrc";

        location loc("previous/filename", 987);
        REQUIRE( !parse_location(path, loc) );
        REQUIRE( loc.get_filename() == "previous/filename" );
        REQUIRE( loc.get_position() == 987 );
    }
}

TEST_CASE("image location to ostream", "[image location]")
{
    std::ostringstream oss;

    SECTION("without position")
    {
        oss << location("example/location", location::no_position);
        REQUIRE( oss.str() == "example/location" );
    }

    SECTION("with position")
    {
        oss << location("example/location", 1234);
        REQUIRE( oss.str() == "1234@example/location" );
    }
}
