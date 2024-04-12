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
 * @file test_trigonometry.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for trigonometry.hpp
 * @date 2024-04-12
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/math/trigonometry.hpp>

#include <xmipp4/math/constants.hpp>

#include <limits>
#include <vector>
#include <map>

using namespace xmipp4::math;

TEST_CASE( "cos", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 1.0},
        {0.1, 0.99500416527},
        {2.0, -0.41614683654},
        {4.2, -0.49026082134},
        {6.0, 0.96017028665},
        {2*pi<double>(), 1.0},
        {pi<double>(), -1.0},
        {pi_4<double>(), sqrt1_2<double>()},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::cos(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::cos(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::cos(-sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::cos(-static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "sin", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.09983341664},
        {2.0, 0.90929742682},
        {4.2, -0.87157577241},
        {6.0, -0.27941549819},
        {pi_2<double>(), 1.0},
        {3*pi_2<double>(), -1.0},
        {pi_4<double>(), sqrt1_2<double>()},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::sin(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::sin(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::sin(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::sin(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}


TEST_CASE( "sincos", "[math]" ) 
{
    const std::vector<double> test_values = 
    {
        0.5, 1.35, 133.0, 12.0, 6.53, -6.26
    };

    for(const auto& value : test_values)
    {
        const auto sc1 = xmipp4::math::sincos(value); 
        REQUIRE( sc1.first == Catch::Approx(sin(value)) );
        REQUIRE( sc1.second == Catch::Approx(cos(value)) );

        const auto sc2 = xmipp4::math::sincos(static_cast<float>(value)); 
        REQUIRE( sc2.first == Catch::Approx(sin(value)) );
        REQUIRE( sc2.second == Catch::Approx(cos(value)) );
    }
}

TEST_CASE( "tan", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.10033467208},
        {2.0, -2.18503986326},
        {4.2, 1.77777977451},
        {6.0, -0.29100619138},
        {pi_4<double>(), 1.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::tan(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::tan(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::tan(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::tan(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "acos", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {-1.0, pi<double>()},
        {-0.5, 2.0943951},
        {-0.1, 1.67096375},
        {0.0, pi_2<double>()},
        {0.1, 1.47062891},
        {0.5, 1.04719755},
        {1.0, 0.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::acos(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::acos(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(xmipp4::math::acos(1.1)) );
    REQUIRE( std::isnan(xmipp4::math::acos(1.1f)) );
    REQUIRE( std::isnan(xmipp4::math::acos(-1.1)) );
    REQUIRE( std::isnan(xmipp4::math::acos(-1.1f)) );
}

TEST_CASE( "asin", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.100167421},
        {0.5, 0.523598776},
        {1.0, pi_2<double>()},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::asin(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::asin(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::asin(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::asin(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }

    REQUIRE( std::isnan(xmipp4::math::asin(1.1)) );
    REQUIRE( std::isnan(xmipp4::math::asin(1.1f)) );
    REQUIRE( std::isnan(xmipp4::math::asin(-1.1)) );
    REQUIRE( std::isnan(xmipp4::math::asin(-1.1f)) );
}

TEST_CASE( "atan", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.0996686525},
        {0.5, 0.463647609},
        {1.0, pi_4<double>()},
        {5.0, 1.37340077},
        {8.0, 1.44644133},
        {100.0, 1.56079666},
        {std::numeric_limits<double>::infinity(), pi_2<double>()},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::atan(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::atan(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::atan(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::atan(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "atan2", "[math]" ) 
{
    const std::map<std::pair<double, double>, double> ground_truth = 
    {
        {{0.0, 0.0}, 0.0},
        {{0.0, 1.0}, 0.0},
        {{1.0, 1.0}, pi_4<double>()},
        {{1.0, 0.0}, pi_2<double>()},
        {{1.0, -1.0}, 3*pi_4<double>()},
        {{0.0, -1.0}, pi<double>()},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::atan2(sample.first.first, sample.first.second) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::atan2(static_cast<float>(sample.first.first), static_cast<float>(sample.first.second)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::atan2(-sample.first.first, sample.first.second) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::atan2(-static_cast<float>(sample.first.first), static_cast<float>(sample.first.second)) == Catch::Approx(-sample.second) );
    }
}
