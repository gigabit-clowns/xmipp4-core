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
        version v(1, 2, 3);
        REQUIRE( v.get_major() == 1 );
        REQUIRE( v.get_minor() == 2 );
        REQUIRE( v.get_patch() == 3 );
    }

    SECTION( "copy constructor" )
    {
        version v(1, 2, 3);
        version v2(v);
        REQUIRE( v2.get_major() == 1 );
        REQUIRE( v2.get_minor() == 2 );
        REQUIRE( v2.get_patch() == 3 );
    }
}

TEST_CASE( "version setters and getters", "[version]" ) 
{
    version v(1, 2, 3);
    v.set_major(4);
    v.set_minor(5);
    v.set_patch(6);
    REQUIRE( v.get_major() == 4 );
    REQUIRE( v.get_minor() == 5 );
    REQUIRE( v.get_patch() == 6 );
}
