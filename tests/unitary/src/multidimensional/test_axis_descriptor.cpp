// SPDX-License-Identifier: GPL-3.0-only

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
