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
 * @file test_arithmetic.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for arithmetic.hpp
 * @date 2024-04-15
 * 
 */

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
