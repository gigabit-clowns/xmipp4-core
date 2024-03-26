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
 * @file test_version.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for version.hpp
 * @date 2023-08-12
 */


#include <catch2/catch_test_macros.hpp>

#include <xmipp4/version.hpp>

using namespace xmipp4;


TEST_CASE( "version constructor and getters", "[version]" ) 
{
    SECTION( "piecewise constructor" )
    {
        version v(1234, 567, 890);
        REQUIRE( v.get_major() == 1234 );
        REQUIRE( v.get_minor() == 567 );
        REQUIRE( v.get_patch() == 890 );
    }

    SECTION( "copy constructor" )
    {
        version v(2345, 678, 901);
        version v2(v);
        REQUIRE( v2.get_major() == 2345 );
        REQUIRE( v2.get_minor() == 678 );
        REQUIRE( v2.get_patch() == 901 );
    }
}

TEST_CASE( "version setters and getters", "[version]" ) 
{
    version v(1, 2, 3);
    v.set_major(3971);
    v.set_minor(999);
    v.set_patch(997);
    REQUIRE( v.get_major() == 3971 );
    REQUIRE( v.get_minor() == 999 );
    REQUIRE( v.get_patch() == 997 );
}

TEST_CASE( "version overflow", "[version]" ) 
{
    version v(4096, 1024, 1024);
    REQUIRE( v.get_major() == 0 );
    REQUIRE( v.get_minor() == 0 );
    REQUIRE( v.get_patch() == 0 );

    v.set_major(8193);
    v.set_minor(2049);
    v.set_patch(2049);
    REQUIRE( v.get_major() == 1 );
    REQUIRE( v.get_minor() == 1 );
    REQUIRE( v.get_patch() == 1 );
}
