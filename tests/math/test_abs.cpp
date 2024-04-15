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
 * @file test_abs.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for abs.hpp
 * @date 2024-04-11
 * 
 */

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
