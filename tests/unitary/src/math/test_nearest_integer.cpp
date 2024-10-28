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
 * @file test_nearest_integer.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for nearest_integer.hpp
 * @date 2024-04-15
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/nearest_integer.hpp>

#include <map>

using namespace xmipp4::math;


TEST_CASE( "trunc", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {1.0, 1.0},
        {5.24, 5.0},
        {5.5, 5.0},
        {5.78, 5.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::trunc(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::trunc(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::trunc(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::trunc(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "floor", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {1.0, 1.0},
        {5.24, 5.0},
        {5.5, 5.0},
        {5.78, 5.0},
        {-1.0, -1.0},
        {-5.24, -6.0},
        {-5.5, -6.0},
        {-5.78, -6.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::floor(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::floor(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "ceil", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {1.0, 1.0},
        {5.24, 6.0},
        {5.5, 6.0},
        {5.78, 6.0},
        {-1.0, -1.0},
        {-5.24, -5.0},
        {-5.5, -5.0},
        {-5.78, -5.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::ceil(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::ceil(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "round", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {1.0, 1.0},
        {5.24, 5.0},
        {5.5, 6.0},
        {5.78, 6.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::round(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::round(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::round(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::round(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}
