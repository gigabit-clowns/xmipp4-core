// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file test_bessel.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Tests for bessel.hpp
 * @date 2024-04-11
 * 
 */

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/bessel.hpp>

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

    REQUIRE( std::isnan(cylindrical_bessel_y0(-1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_y0(-1.0f)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(0, -1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(0, -1.0f)) );
}

TEST_CASE( "cylindrical_bessel_y1", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        // {0.0, -std::numeric_limits<double>::infinity()}, // On Windows returns NaN
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
    REQUIRE( std::isnan(cylindrical_bessel_yn(1, -1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(1, -1.0f)) );
}

TEST_CASE( "cylindrical_bessel_yn", "[math]" ) 
{
    const std::map<std::pair<int, double>, double> ground_truth = 
    {
        // {{2, 0.0}, -std::numeric_limits<double>::infinity()}, // On Windows returns NaN
        // {{3, 0.0}, -std::numeric_limits<double>::infinity()}, // On Windows returns NaN
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

TEST_CASE( "cylindrical_bessel_i0", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 1.0},
        {0.1, 1.002501562934095},
        {0.5, 1.063483370741324},
        {1.0, 1.266065877752008},
        {5.0, 27.239871823604450},
        {10.0, 2.815716628466255e+03},
        {20.0, 4.355828255955355e+07},
        {50.0, 2.932553783849337e+20},

    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_i0(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_i0(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_i0(-sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_i0(-static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(0, sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(0, static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(0, -sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(0, -static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "cylindrical_bessel_i1", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.050062526047093},
        {0.5, 0.257894305390896},
        {1.0, 0.565159103992485},
        {5.0, 24.335642142450530},
        {10.0, 2.670988303701255e+03},
        {20.0, 4.245497338512778e+07},
        {50.0, 2.903078590103557e+20},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_i1(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_i1(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_i1(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( cylindrical_bessel_i1(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
        REQUIRE( cylindrical_bessel_in(1, sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(1, static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(1, -sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( cylindrical_bessel_in(1, -static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "cylindrical_bessel_in", "[math]" ) 
{
    const std::map<std::pair<int, double>, double> ground_truth = 
    {
        {{2, 0.0}, 0.0},
        {{3, 0.0}, 0.0},
        {{2, 1.0}, 0.135747669767038},
        {{2, -1.0}, 0.135747669767038},
        {{4, 10.0}, 1.226490537759491e+03},
        {{4, -10.0}, 1.226490537759491e+03},
        {{2, 0.1}, 0.001251041992242},
        {{10, 0.5}, 2.643041925881278e-13},
        {{4, 0.5}, 1.648055498548234e-04},
        {{3, 1.0},  0.022168424924332},
        {{3, -1.0},  -0.022168424924332},
        {{3, 5.0}, 10.331150169151138},
        {{3, -5.0}, -10.331150169151138},
        {{4, 5.0}, 5.108234763642869},
        {{4, -5.0}, 5.108234763642869},
        {{5, 5.0}, 2.157974547322546},
        {{5, -5.0}, -2.157974547322546},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_in(sample.first.first, sample.first.second) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(sample.first.first, static_cast<float>(sample.first.second)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(-sample.first.first, sample.first.second) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_in(-sample.first.first, static_cast<float>(sample.first.second)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "cylindrical_bessel_k0", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, std::numeric_limits<double>::infinity()},
        {0.1, 2.427069024702017},
        {0.5, 0.924419071227666},
        {1.0, 0.421024438240708},
        {5.0, 0.003691098334043},
        {10.0, 1.778006231616765e-05},
        {50.0, 3.410167749789496e-23},

    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_k0(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_k0(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_kn(0, sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_kn(0, static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(cylindrical_bessel_k0(-1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_k0(-1.0f)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(0, -1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(0, -1.0f)) );
}

TEST_CASE( "cylindrical_bessel_k1", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, std::numeric_limits<double>::infinity()},
        {0.1, 9.853844780870606},
        {0.5, 1.656441120003301},
        {1.0, 0.601907230197235},
        {5.0, 0.004044613445452},
        {10.0, 1.864877345382559e-05},
        {50.0, 3.444102226717556e-23},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_k1(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_k1(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_kn(1, sample.first) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_kn(1, static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(cylindrical_bessel_k1(-1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_k1(-1.0f)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(1, -1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(1, -1.0f)) );
}

TEST_CASE( "cylindrical_bessel_kn", "[math]" ) 
{
    const std::map<std::pair<int, double>, double> ground_truth = 
    {
        {{2, 0.0}, std::numeric_limits<double>::infinity()},
        {{3, 0.0}, std::numeric_limits<double>::infinity()},
        {{2, 1.0}, 1.624838898635177},
        {{4, 10.0}, 3.786143716089198e-05},
        {{2, 0.1}, 1.995039646421141e+02},
        {{10, 0.5}, 1.889375693199002e+11},
        {{4, 0.5}, 7.522450979104038e+02},
        {{3, 1.0}, 7.101262824737944},
        {{3, 5.0}, 0.008291768415231},
        {{4, 5.0}, 0.015259065810501},
        {{5, 5.0}, 0.032706273712032},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( cylindrical_bessel_kn(sample.first.first, sample.first.second) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_kn(sample.first.first, static_cast<float>(sample.first.second)) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_kn(-sample.first.first, sample.first.second) == Catch::Approx(sample.second) );
        REQUIRE( cylindrical_bessel_kn(-sample.first.first, static_cast<float>(sample.first.second)) == Catch::Approx(sample.second) );
    }

    REQUIRE( std::isnan(cylindrical_bessel_kn(2, -1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(2, -1.0f)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(-3, -1.0)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(-3, -1.0f)) );
}
