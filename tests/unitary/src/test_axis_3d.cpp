// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/axis_3d.hpp>

using namespace xmipp4;


TEST_CASE( "operator-", "[axis_3d]" ) 
{
    axis_3d input;
    axis_3d expected;
    std::tie(input, expected) = GENERATE(
        table<axis_3d, axis_3d>({
            {axis_3d::zero, axis_3d::zero},
            {axis_3d::x, axis_3d::negative_x},
            {axis_3d::y, axis_3d::negative_y},
            {axis_3d::z, axis_3d::negative_z},
            {axis_3d::negative_x, axis_3d::x},
            {axis_3d::negative_y, axis_3d::y},
            {axis_3d::negative_z, axis_3d::z}
        })
    );

    REQUIRE( -input == expected );
}

TEST_CASE( "is_positive should return true with positive axis_3d", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::x,
        axis_3d::y,
        axis_3d::z
    );

    REQUIRE( is_positive(axis) == true );
}

TEST_CASE( "is_positive should return false with non positive axis_3d", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::zero,
        axis_3d::negative_x,
        axis_3d::negative_y,
        axis_3d::negative_z
    );

    REQUIRE( is_positive(axis) == false );
}

TEST_CASE( "is_negative should return true with negative axis_3d", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::negative_x,
        axis_3d::negative_y,
        axis_3d::negative_z
    );

    REQUIRE( is_negative(axis) == true );
}

TEST_CASE( "is_negative should return false with non negative axis_3d", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::zero,
        axis_3d::x,
        axis_3d::y,
        axis_3d::z
    );

    REQUIRE( is_negative(axis) == false );
}

TEST_CASE( "next_axis should return the next axis_3d in sequence", "[axis_3d]" ) 
{
    axis_3d input, expected;
    std::tie(input, expected) = GENERATE(
        table<axis_3d, axis_3d>({
            {axis_3d::zero, axis_3d::zero},
            {axis_3d::x, axis_3d::y},
            {axis_3d::y, axis_3d::z},
            {axis_3d::z, axis_3d::x},
            {axis_3d::negative_x, axis_3d::negative_z},
            {axis_3d::negative_y, axis_3d::negative_x},
            {axis_3d::negative_z, axis_3d::negative_y}
        })
    );

    REQUIRE( next_axis(input) == expected );
}

TEST_CASE( "dot product with axis_3d should return zero when one of the inputs is zero", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::zero,
        axis_3d::x,
        axis_3d::y,
        axis_3d::z
    );
    const auto zero = axis_3d::zero;

    REQUIRE( dot(axis, zero) == 0 );
    REQUIRE( dot(zero, axis) == 0 );
    REQUIRE( dot(-axis, zero) == 0 );
    REQUIRE( dot(zero, -axis) == 0 );
}
    
TEST_CASE( "dot product with axis_3d should return one when both axis_3d-s are non-zero and equal", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::x,
        axis_3d::y,
        axis_3d::z
    );

    REQUIRE( dot(axis, axis) == 1 );
    REQUIRE( dot(-axis, -axis) == 1 );
}

TEST_CASE( "dot product with axis_3d should return minus one when both axis_3d-s are opposite", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::x,
        axis_3d::y,
        axis_3d::z
    );

    REQUIRE( dot(axis, -axis) == -1 );
    REQUIRE( dot(-axis, axis) == -1 );
}

TEST_CASE( "dot product with axis_3d should return zero when axis_3d-s are orthogonal", "[axis_3d]" ) 
{
    axis_3d a, b;
    std::tie(a, b) = GENERATE(
        table<axis_3d, axis_3d>({
            {axis_3d::x, axis_3d::y},
            {axis_3d::z, axis_3d::x},
            {axis_3d::y, axis_3d::negative_x},
            {axis_3d::negative_z, axis_3d::negative_y},
            {axis_3d::x, axis_3d::negative_y}
        })
    );


    REQUIRE( dot(a, b) == 0 );
    REQUIRE( dot(b, a) == 0 );
}

TEST_CASE( "cross product with axis_3d should return zero when one of the inputs is zero", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::zero,
        axis_3d::x,
        axis_3d::y,
        axis_3d::z,
        axis_3d::negative_x,
        axis_3d::negative_y,
        axis_3d::negative_z
    );
    const auto zero = axis_3d::zero;

    REQUIRE( cross(axis, zero) == axis_3d::zero );
    REQUIRE( cross(zero, axis) == axis_3d::zero );
}
    
TEST_CASE( "cross product with axis_3d should return zero when both axis_3d-s are equal or opposite", "[axis_3d]" ) 
{
    const auto axis = GENERATE(
        axis_3d::x,
        axis_3d::y,
        axis_3d::z
    );

    REQUIRE( cross(axis, axis) == axis_3d::zero );
    REQUIRE( cross(axis, -axis) == axis_3d::zero );
    REQUIRE( cross(-axis, axis) == axis_3d::zero );
    REQUIRE( cross(-axis, -axis) == axis_3d::zero );
}

TEST_CASE( "cross product with othrogonal axis_3d should return the correct result", "[axis_3d]" ) 
{
    axis_3d a, b, expected;
    std::tie(a, b, expected) = GENERATE(
        table<axis_3d, axis_3d, axis_3d>({
            {axis_3d::x, axis_3d::y, axis_3d::z},
            {axis_3d::y, axis_3d::z, axis_3d::x},
            {axis_3d::z, axis_3d::x, axis_3d::y}
        })
    );

    REQUIRE( cross(a, b) == expected );
    REQUIRE( cross(a, -b) == -expected );
    REQUIRE( cross(-a, b) == -expected );
    REQUIRE( cross(-a, -b) == expected );

    REQUIRE( cross(b, a) == -expected );
    REQUIRE( cross(b, -a) == expected );
    REQUIRE( cross(-b, a) == expected );
    REQUIRE( cross(-b, -a) == -expected );
}

TEST_CASE( "to_string with axis_3d should produce correct results", "[axis_3d]" ) 
{
    axis_3d input;
    std::string expected;
    std::tie(input, expected) = GENERATE(
        table<axis_3d, std::string>({
            {axis_3d::x, "x"},
            {axis_3d::y, "y"},
            {axis_3d::z, "z"},
            {axis_3d::zero, "0"},
            {axis_3d::negative_x, "-x"},
            {axis_3d::negative_y, "-y"},
            {axis_3d::negative_z, "-z"}
        })
    );

    REQUIRE( std::string(to_string(input)) == expected );
}

TEST_CASE( "from_string with axis_3d should produce correct results", "[axis_3d]" ) 
{
    std::string input;
    axis_3d expected_axis;
    bool expected_result;
    std::tie(input, expected_axis, expected_result) = GENERATE(
        table<std::string, axis_3d, bool>({
            {"0", axis_3d::zero, true},
            {"zero", axis_3d::zero, true},
            {"x", axis_3d::x, true},
            {"+x", axis_3d::x, true},
            {"y", axis_3d::y, true},
            {"+y", axis_3d::y, true},
            {"z", axis_3d::z, true},
            {"+z", axis_3d::z, true},
            {"-x", axis_3d::negative_x, true},
            {"-y", axis_3d::negative_y, true},
            {"-z", axis_3d::negative_z, true},
            {"invalid", axis_3d::zero, false}
        })
    );

    axis_3d axis;
    bool result = from_string(input, axis);
    REQUIRE( result == expected_result );
    if (result)
    {
        REQUIRE( axis == expected_axis );
    }
}
