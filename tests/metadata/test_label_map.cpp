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

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/metadata/label_map.hpp>

#include <algorithm>
#include <list>

using namespace xmipp4;

using label_map = xmipp4::metadata::label_map<std::string>; // Using string as the mapped type

TEST_CASE( "Construct label map with unique elements", "[label_map]" )
{
    const std::list<label_map::value_type> values = 
    {
        { "hello", "good bye" },
        { "this is", "a test" },
        { "I hope", "it completes"},
        { "without", "errors"}
    };
    label_map map(values.begin(), values.end());

    REQUIRE( std::equal(map.begin(), map.end(), values.begin(), values.end()) );
    REQUIRE( map.at("hello") == "good bye" );
    REQUIRE( map.at("this is") == "a test" );
    REQUIRE( map.at("I hope") == "it completes" );
    REQUIRE( map.at("without") == "errors" );

    SECTION ("Copy constructor")
    {
        label_map map2(map);
        REQUIRE( std::equal(map.begin(), map.end(), values.begin(), values.end()) );
    }
}

TEST_CASE( "Construct label map with duplicate elements", "[label_map]" )
{
    std::list<label_map::value_type> values = 
    {
        { "hello", "good bye" },
        { "this is", "a test" },
        { "this is", "a repeated element"},
        { "hello", "i am repeated as well"},
        { "I hope", "it completes"},
        { "without", "errors"}
    };
    label_map map(values.begin(), values.end());

    // Make values unique
    values.erase(std::next(values.begin(), 2), std::next(values.begin(), 4));
   
    REQUIRE( std::equal(map.begin(), map.end(), values.begin(), values.end()) );
    REQUIRE( map.at("hello") == "good bye" );
    REQUIRE( map.at("this is") == "a test" );
    REQUIRE( map.at("I hope") == "it completes" );
    REQUIRE( map.at("without") == "errors" );

    SECTION ("Copy constructor")
    {
        label_map map2(map);
        REQUIRE( std::equal(map.begin(), map.end(), values.begin(), values.end()) );
    }
}
