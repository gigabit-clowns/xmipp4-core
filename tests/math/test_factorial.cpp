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
 * @file test_factorial.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for factorial.hpp
 * @date 2024-04-12
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/math/factorial.hpp>

#include <limits>
#include <cstdint>

using namespace xmipp4::math;

TEST_CASE( "factorial", "[math]" ) 
{
    REQUIRE( factorial(0U) == 1U );
    REQUIRE( factorial(1U) == 1U );
    REQUIRE( factorial(2U) == 2U );
    REQUIRE( factorial(5U) == 120U );
    REQUIRE( factorial(5U) == 120U );
    REQUIRE( factorial(uint8_t(6)) == 208 ); // Overflow
}

TEST_CASE( "large_factorial", "[math]" ) 
{
    REQUIRE( large_factorial<double>(0U) == Catch::Approx(1.0) );
    REQUIRE( large_factorial<double>(1U) == Catch::Approx(1.0) );
    REQUIRE( large_factorial<double>(2U) == Catch::Approx(2.0) );
    REQUIRE( large_factorial<double>(16U) == Catch::Approx(2.092279e+13) );
}

TEST_CASE( "gamma", "[math]" ) 
{
    REQUIRE( xmipp4::math::gamma(0.0) == std::numeric_limits<double>::infinity() );
    REQUIRE( xmipp4::math::gamma(1.0) == Catch::Approx(1.0) );
    REQUIRE( xmipp4::math::gamma(2.0) == Catch::Approx(1.0) );
    REQUIRE( xmipp4::math::gamma(16.0) == Catch::Approx(1.307674368e+12) );
    REQUIRE( xmipp4::math::gamma(-1.5) == Catch::Approx(2.363271801207355) );
}
