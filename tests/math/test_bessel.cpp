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
 * @file test_bessel.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for bessel.hpp
 * @date 2024-04-11
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/math/bessel.hpp>

#include <map>
#include <cmath>

using namespace xmipp4::math;


TEST_CASE( "cylindrical_bessel_j0", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 1.0},
        {0.1, 0.997501562066040},
        {0.5, 0.938469807240813},
        {1.0, 0.765197686557967},
        {5.0, -0.177596771314338},
        {10.0, -0.245935764451348},
        {20.0, 0.167024664340583},
        {50.0, 0.055812327669252},
        {100.0, 0.019985850304223},

    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_j0(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_j0(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_j0(-sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_j0(-static_cast<float>(sample.first)) == Catch::Approx(sample.second) );

    
        REQUIRE( cylindrical_bessel_jn(0, sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_jn(0, static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_jn(0, -sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_jn(0, -static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "cylindrical_bessel_j1", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.049937526036242},
        {0.5, 0.242268457674874},
        {1.0, 0.440050585744934},
        {5.0, -0.327579137591465},
        {10.0, 0.043472746168861},
        {20.0, 0.066833124175850},
        {50.0, -0.097511828125175},
        {100.0, -0.077145352014112},

    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_j1(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_j1(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_j1(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( cylindrical_bessel_j1(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );

        REQUIRE( cylindrical_bessel_jn(1, sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_jn(1, static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_jn(1, -sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( cylindrical_bessel_jn(1, -static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "cylindrical_bessel_jn", "[math]" ) 
{
    const std::map<std::pair<int, double>, double> ground_truth = 
    {
        {{3, 0.0}, 0.0},
        {{2, 1.0}, 0.114903484931901},
        {{4, 10.0}, -0.219602686102008},
        {{2, 0.1}, 0.001248958658800},
        {{10, 0.5}, 2.613177360822802e-13},
        {{4, 0.5}, 1.607364763642876e-04},
        {{3, 1.0}, 0.019563353982668},
        {{3, 100.0}, 0.076284201720332},
        {{4, 100.0}, 0.026105809447725},
        {{5, 100.0}, -0.074195736964514},
        {{3, -100.0}, -0.076284201720332},
        {{4, -100.0}, 0.026105809447725},
        {{5, -100.0}, 0.074195736964514},
        {{4, -10.0}, -0.219602686102008},
        {{5, -10.0}, 0.234061528186794},
        {{-2, 1.0}, 0.114903484931901},
        {{-3, 1.0}, -0.019563353982668},
        {{-2, -1.0}, 0.114903484931901},
        {{-3, -1.0}, 0.019563353982668},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_jn(sample.first.first, sample.first.second) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_jn(sample.first.first, static_cast<float>(sample.first.second)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "cylindrical_bessel_y0", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, -std::numeric_limits<double>::infinity()},
        {0.1, -1.534238651350367},
        {0.5, -0.444518733506707},
        {1.0, 0.088256964215677},
        {5.0, -0.308517625249034},
        {10.0, 0.055671167283599},
        {20.0, 0.062640596809384},
        {50.0, -0.098064995470077},
        {100.0, -0.077244313365083},

    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_y0(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_y0(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_yn(0, sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_yn(0, static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(cylindrical_bessel_y1(-1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_y1(-1.0f)) );
}

TEST_CASE( "cylindrical_bessel_y1", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.1, -6.458951094702027},
        {0.5, -1.471472392670243},
        {1.0, -0.781212821300289},
        {5.0, 0.147863143391227},
        {10.0, 0.249015424206954},
        {20.0, -0.165511614362521},
        {50.0, -0.056795668562015},
        {100.0, -0.020372312002760},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_y1(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_y1(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_yn(1, sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_yn(1, static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(cylindrical_bessel_y1(-1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_y1(-1.0f)) );
}

TEST_CASE( "cylindrical_bessel_yn", "[math]" ) 
{
    const std::map<std::pair<int, double>, double> ground_truth = 
    {
        {{2, 1.0}, -1.650682606816255},
        {{4, 10.0}, -0.144949511868094},
        {{2, 0.1}, -1.276447832426902e+02},
        {{10, 0.5}, -1.219636233495696e+11},
        {{4, 0.5}, -4.992725608195124e+02},
        {{3, 1.0}, -5.821517605964731},
        {{3, 100.0}, 0.023445786687761},
        {{4, 100.0}, -0.075430119923762},
        {{5, 100.0}, -0.029480196281662},
        {{-2, 1.0}, -1.650682606816255},
        {{-3, 1.0}, 5.821517605964731},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_yn(sample.first.first, sample.first.second) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_yn(sample.first.first, static_cast<float>(sample.first.second)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(cylindrical_bessel_yn(2, -1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(2, -1.0f)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(-3, -1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(-3, -1.0f)) );
}
