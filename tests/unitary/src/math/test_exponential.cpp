// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/exponential.hpp>

#include <map>
#include <limits>
#include <cstdint>

using namespace xmipp4::math;

TEST_CASE( "exp", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {-20.0, 2.061153622438558e-09},
        {-10.0, 4.539992976248485e-05},
        {-1.0, 0.367879441171442},
        {0.0, 1.0},
        {0.5, 1.648721270700128},
        {10.0, 2.202646579480672e+04},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::exp(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::exp(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "exp2", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {-20.0, 9.536743164062500e-07},
        {-10.0, 9.765625000000000e-04},
        {-1.0, 0.5},
        {0.0, 1.0},
        {0.5, 1.414213562373095},
        {10.0, 1024.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::exp2(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::exp2(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "exp10", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {-20.0, 1e-20},
        {-10.0, 1e-10},
        {-1.0, 0.1},
        {0.0, 1.0},
        {0.5, 3.162277660168380},
        {10.0, 1e10},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::exp10(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::exp10(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }
}

TEST_CASE( "log", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.01, -4.605170185988091},
        {0.1, -2.302585092994046},
        {0.5, -0.693147180559945},
        {1.0, 0.0},
        {5.0, 1.609437912434100},
        {10.0, 2.302585092994046},
        {50.0, 3.912023005428146},
        {1e6, 13.815510557964274},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::log(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::log(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( xmipp4::math::log(0.0) == -std::numeric_limits<double>::infinity() );
    REQUIRE( xmipp4::math::log(0.0f) == -std::numeric_limits<float>::infinity() );
    REQUIRE( std::isnan(xmipp4::math::log(-1.0)) );
    REQUIRE( std::isnan(xmipp4::math::log(-1.0f)) );
}

TEST_CASE( "log2", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.01, -6.643856189774724},
        {0.1, -3.321928094887362},
        {0.5, -1.0},
        {1.0, 0.0},
        {2.0, 1.0},
        {5.0, 2.321928094887362},
        {10.0, 3.321928094887362},
        {1e6, 19.931568569324174},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::log2(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::log2(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( xmipp4::math::log2(0.0) == -std::numeric_limits<double>::infinity() );
    REQUIRE( xmipp4::math::log2(0.0f) == -std::numeric_limits<float>::infinity() );
    REQUIRE( std::isnan(xmipp4::math::log2(-1.0)) );
    REQUIRE( std::isnan(xmipp4::math::log2(-1.0f)) );
}

TEST_CASE( "log10", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.01, -2.0},
        {0.1, -1.0},
        {0.5, -0.301029995663981},
        {1.0, 0.0},
        {2.0, 0.301029995663981},
        {5.0, 0.698970004336019},
        {10.0, 1.0},
        {1e6, 6.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::log10(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::log10(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
    }

    REQUIRE( xmipp4::math::log10(0.0) == -std::numeric_limits<double>::infinity() );
    REQUIRE( xmipp4::math::log10(0.0f) == -std::numeric_limits<float>::infinity() );
    REQUIRE( std::isnan(xmipp4::math::log10(-1.0)) );
    REQUIRE( std::isnan(xmipp4::math::log10(-1.0f)) );
}
