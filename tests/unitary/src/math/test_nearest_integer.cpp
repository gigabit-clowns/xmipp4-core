// SPDX-License-Identifier: GPL-3.0-only

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
