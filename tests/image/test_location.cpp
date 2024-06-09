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

#include <vector>
#include <string>
#include <sstream>

using namespace xmipp4::image;

TEST_CASE("find contiguous location run", "[image location]")
{
    SECTION("no postion")
    {
        const std::vector<location> data = {
            location("aaaa", location::no_position),
            location("aaaa", location::no_position),
            location("bbbbb", location::no_position),
        };

        REQUIRE( std::distance(data.begin(), find_contiguous_location_run(data.begin(), data.end())) == 1 );
    }

    SECTION("position jump")
    {
        const std::vector<location> data = {
            location("aaaa", 1),
            location("aaaa", 2),
            location("aaaa", 3),
            location("aaaa", 4),
            location("aaaa", 6),
        };

        REQUIRE( std::distance(data.begin(), find_contiguous_location_run(data.begin(), data.end())) == 4 );
    }
    
    SECTION("different file")
    {
        const std::vector<location> data = {
            location("aaaa", 1),
            location("aaaa", 2),
            location("aaaa", 3),
            location("bbbb", 4),
        };

        REQUIRE( std::distance(data.begin(), find_contiguous_location_run(data.begin(), data.end())) == 3 );
    }

    SECTION("fully contiguous")
    {
        const std::vector<location> data = {
            location("aaaa", 12),
            location("aaaa", 13),
            location("aaaa", 14),
            location("aaaa", 15),
            location("aaaa", 16),
            location("aaaa", 17),
        };

        REQUIRE( std::distance(data.begin(), find_contiguous_location_run(data.begin(), data.end())) == 6 );
    }
}

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
