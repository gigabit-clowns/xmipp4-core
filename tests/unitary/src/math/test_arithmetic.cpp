// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/arithmetic.hpp>

using namespace xmipp4::math;


TEST_CASE( "multiply_add", "[math]" ) 
{
    REQUIRE( xmipp4::math::multiply_add(2.31, 1.35, 2.21) == Catch::Approx(5.3285) );
    REQUIRE( xmipp4::math::multiply_add(2.31f, 1.35f, 2.21f) == Catch::Approx(5.3285f) );

    REQUIRE( xmipp4::math::multiply_add(-2.5252, 5.23565, 73.2121) == Catch::Approx(59.99103662) );
    REQUIRE( xmipp4::math::multiply_add(-2.5252f, 5.23565f, 73.2121f) == Catch::Approx(59.99103662) );
}

TEST_CASE( "mod", "[math]" ) 
{
    REQUIRE( xmipp4::math::mod(2.31, 1.35) == Catch::Approx(0.96) );
    REQUIRE( xmipp4::math::mod(2.31f, 1.35f) == Catch::Approx(0.96) );
    
    REQUIRE( xmipp4::math::mod(2.31, -1.35) == Catch::Approx(0.96) );
    REQUIRE( xmipp4::math::mod(2.31f, -1.35f) == Catch::Approx(0.96) );
}
