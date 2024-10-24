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
 * @file test_bspline.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for bspline.hpp
 * @date 2024-10-24
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/bspline.hpp>

#include <map>

using namespace xmipp4::math;


TEST_CASE( "bspline0", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 1.0},
        {0.25, 1.0},
        {0.75, 0.0},
        {1.0, 0.0},
        {10.0, 0.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( bspline0(+sample.first) == Catch::Approx(sample.second) );
        REQUIRE( bspline0(-sample.first) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "bspline1", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 1.0},
        {0.25, 0.75},
        {0.5, 0.5},
        {0.75, 0.25},
        {1.0, 0.0},
        {10.0, 0.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( bspline1(+sample.first) == Catch::Approx(sample.second) );
        REQUIRE( bspline1(-sample.first) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "bspline2", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.75},
        {0.25, 0.6875},
        {0.5, 0.5},
        {0.75, 0.28125},
        {1.0, 0.125},
        {1.25, 0.03125},
        {1.5, 0.0},
        {2.0, 0.0},
        {10.0, 0.0},

    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( bspline2(+sample.first) == Catch::Approx(sample.second) );
        REQUIRE( bspline2(-sample.first) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "bspline3", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 2.0/3.0},
        {0.5, 0.4791666666666666666666},
        {1.0, 0.1666666666666666666666},
        {1.5, 0.0208333333333333333333},
        {2.0, 0.0},
        {2.5, 0.0},
        {10.0, 0.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( bspline3(+sample.first) == Catch::Approx(sample.second) );
        REQUIRE( bspline3(-sample.first) == Catch::Approx(sample.second) );
    }
}
