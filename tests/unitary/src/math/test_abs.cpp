// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/math/abs.hpp>

#include <limits>
#include <cstdint>

using namespace xmipp4::math;


TEST_CASE( "abs", "[math]" ) 
{
    REQUIRE( abs(static_cast<uint8_t>(1)) == 1 );
    REQUIRE( abs(static_cast<uint8_t>(255)) == 255 );
    REQUIRE( abs(static_cast<uint64_t>(1)) == 1 );
    REQUIRE( abs(static_cast<uint64_t>(545465)) == 545465 );

    REQUIRE( abs(static_cast<int8_t>(0)) == 0 );
    REQUIRE( abs(static_cast<int8_t>(1)) == 1 );
    REQUIRE( abs(static_cast<int8_t>(-128)) == 128 );
    REQUIRE( abs(static_cast<int64_t>(0)) == 0 );
    REQUIRE( abs(static_cast<int64_t>(1)) == 1 );
    REQUIRE( abs(static_cast<int64_t>(-643214)) == 643214 );

    REQUIRE( abs(0.0f) == 0.0f );
    REQUIRE( abs(1.0f) == 1.0f );
    REQUIRE( abs(-1.0f) == 1.0f );
    REQUIRE( abs(-std::numeric_limits<float>::infinity()) == std::numeric_limits<float>::infinity() );
    REQUIRE( abs(0.0) == 0.0 );
    REQUIRE( abs(1.0) == 1.0 );
    REQUIRE( abs(-1.0) == 1.0 );
    REQUIRE( abs(-std::numeric_limits<double>::infinity()) == std::numeric_limits<double>::infinity() );
}
