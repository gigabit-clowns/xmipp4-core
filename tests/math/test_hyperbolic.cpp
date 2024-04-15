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
 * @file test_hyperbolic.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for hyperbolic.hpp
 * @date 2024-04-15
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/math/hyperbolic.hpp>

#include <xmipp4/math/constants.hpp>

#include <limits>
#include <map>

using namespace xmipp4::math;

TEST_CASE( "cosh", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 1.0},
        {0.1, 1.00500416806},
        {2.0, 3.76219569108},
        {4.2, 33.3506633089},
        {6.0, 201.715636122},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::cosh(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::cosh(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::cosh(-sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::cosh(-static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "sinh", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.10016675002},
        {2.0, 3.62686040785},
        {4.2, 33.3356677321},
        {6.0, 201.71315737},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::sinh(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::sinh(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::sinh(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::sinh(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "tanh", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.09966799462},
        {2.0, 0.96402758007},
        {4.2, 0.99955036646},
        {6.0, 0.99998771165},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::tanh(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::tanh(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::tanh(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::tanh(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "acosh", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {1.0, 0.0},
        {1.5, 0.962423650119207},
        {10.0, 2.993222846126381},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::acosh(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::acosh(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(xmipp4::math::acosh(0.5)) );
    REQUIRE( std::isnan(xmipp4::math::acosh(0.5f)) );
    REQUIRE( std::isnan(xmipp4::math::acosh(-1.0)) );
    REQUIRE( std::isnan(xmipp4::math::acosh(-1.0f)) );
}

TEST_CASE( "asinh", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.099834078899208},
        {1.0, 0.881373587019543},
        {10.0, 2.998222950297970},
        {100.0, 5.298342365610589},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::asinh(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::asinh(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::asinh(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::asinh(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "atanh", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.100335347731076},
        {0.5, 0.549306144334055},
        {0.9, 1.472219489583220},
        {1.0, std::numeric_limits<double>::infinity()},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::atanh(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::atanh(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::atanh(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::atanh(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }

    REQUIRE( std::isnan(xmipp4::math::atanh(1.1)) );
    REQUIRE( std::isnan(xmipp4::math::atanh(1.1f)) );
    REQUIRE( std::isnan(xmipp4::math::atanh(-1.1)) );
    REQUIRE( std::isnan(xmipp4::math::atanh(-1.1f)) );
}
