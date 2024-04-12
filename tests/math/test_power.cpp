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
 * @file test_power.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for power.hpp
 * @date 2024-04-12
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/math/power.hpp>

#include <xmipp4/math/constants.hpp>

#include <utility>
#include <map>

using namespace xmipp4::math;

TEST_CASE( "square", "[math]" ) 
{
    REQUIRE( xmipp4::math::square(0) == 0 );
    REQUIRE( xmipp4::math::square(0.0) == 0.0 );
    REQUIRE( xmipp4::math::square(0.0f) == 0.0f );
    REQUIRE( xmipp4::math::square(-1) == 1 );
    REQUIRE( xmipp4::math::square(-2) == 4 );
    REQUIRE( xmipp4::math::square(16) == 256 );
    REQUIRE( xmipp4::math::square(16U) == 256U );
    REQUIRE( xmipp4::math::square(16.0) == 256.0 );
    REQUIRE( xmipp4::math::square(16.0f) == 256.0f );
}

TEST_CASE( "pow", "[math]" ) 
{
    const std::map<std::pair<double, double>, double> ground_truth = 
    {
        {{0.0, 0.0}, 1.0},
        {{1.0, 0.0}, 1.0},
        {{2.0, 0.0}, 1.0},
        {{-1.0, 0.0}, 1.0},
        {{2.0, 2.0}, 4.0},
        {{2.0, -2.0}, 0.25},
        {{2.0, 0.5}, sqrt2<double>()},
        {{4.64, 4.313}, 749.362662244},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::pow(sample.first.first, sample.first.second) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::pow(static_cast<float>(sample.first.first), static_cast<float>(sample.first.second)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "sqrt", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.0144, 0.12},
        {0.01, 0.1},
        {0.5, sqrt1_2<double>()},
        {1.0, 1.0},
        {2.0, sqrt2<double>()},
        {144.0, 12.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::sqrt(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::sqrt(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(xmipp4::math::sqrt(-1.0)) );
    REQUIRE( std::isnan(xmipp4::math::sqrt(-1.0f)) );
}

TEST_CASE( "cbrt", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.001728, 0.12},
        {0.001, 0.1},
        {1.0, 1.0},
        {1e3, 10},
        {1728, 12.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::cbrt(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::cbrt(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::cbrt(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::cbrt(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}
