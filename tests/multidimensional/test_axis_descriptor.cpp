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
 * @file test_axis_descriptor.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for multidimensional/axis_descriptor.hpp
 * @date 2024-05-08
 * 
 */

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/axis_descriptor.hpp>

#include <array>

using namespace xmipp4::multidimensional;

TEST_CASE("broadcast", "[memory_layout]")
{
    SECTION("no axes")
    {
        REQUIRE( broadcast() );
    }
    
    SECTION("one axis")
    {
        axis_descriptor axis0(3, 1);
        REQUIRE( broadcast(axis0) );
        REQUIRE( axis0 == axis_descriptor(3, 1) );
    }

    SECTION("two axes with same size")
    {
        axis_descriptor axis0(3, 1);
        axis_descriptor axis1(3, 2);
        REQUIRE( broadcast(axis0, axis1) );
        REQUIRE( axis0 == axis_descriptor(3, 1) );
        REQUIRE( axis1 == axis_descriptor(3, 2) );
    }

    SECTION("two axes with one axis of size 1")
    {
        axis_descriptor axis0(3, 1);
        axis_descriptor axis1(1, 2);
        REQUIRE( broadcast(axis0, axis1) );
        REQUIRE( axis0 == axis_descriptor(3, 1) );
        REQUIRE( axis1 == axis_descriptor(3, 0) );
    }

    SECTION("two axes with mismatching sizes")
    {
        axis_descriptor axis0(3, 1);
        axis_descriptor axis1(4, 2);
        REQUIRE( !broadcast(axis0, axis1) );
    }

    SECTION("multiple axes")
    {
        axis_descriptor axis0(1, 1);
        axis_descriptor axis1(3, 2);
        axis_descriptor axis2(1, 3);
        axis_descriptor axis3(3, 4);
        REQUIRE( broadcast(axis0, axis1, axis2, axis3) );
        REQUIRE( axis0 == axis_descriptor(3, 0) );
        REQUIRE( axis1 == axis_descriptor(3, 2) );
        REQUIRE( axis2 == axis_descriptor(3, 0) );
        REQUIRE( axis3 == axis_descriptor(3, 4) );
    }

    SECTION("multiple axes with one mismatch")
    {
        axis_descriptor axis0(1, 1);
        axis_descriptor axis1(3, 2);
        axis_descriptor axis2(2, 3);
        axis_descriptor axis3(3, 4);
        REQUIRE( !broadcast(axis0, axis1, axis2, axis3) );
    }
}
