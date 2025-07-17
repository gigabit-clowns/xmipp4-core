// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/erf.hpp>

#include <map>
#include <set>

using namespace xmipp4::math;

TEST_CASE( "erf", "[math]" ) 
{
    const std::map<double, double> ground_truth = 
    {
        {0.0, 0.0},
        {0.1, 0.112462916018285},
        {0.5, 0.520499877813047},
        {1.0, 0.842700792949715},
        {2.0, 0.995322265018953},
        {10.0, 1.0},
    };

    for(const auto& sample : ground_truth)
    {
        REQUIRE( xmipp4::math::erf(sample.first) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::erf(static_cast<float>(sample.first)) == Catch::Approx(sample.second) );
        REQUIRE( xmipp4::math::erf(-sample.first) == Catch::Approx(-sample.second) );
        REQUIRE( xmipp4::math::erf(-static_cast<float>(sample.first)) == Catch::Approx(-sample.second) );
    }
}

TEST_CASE( "erfc", "[math]" ) 
{
    const std::set<double> test_values = 
    {
        0.0, 0.1, 3.5, 1.5, 1.0, 100.0
    };

    for(const auto& sample : test_values)
    {
        REQUIRE( xmipp4::math::erfc(sample) == Catch::Approx(1.0 - xmipp4::math::erf(sample)) );
        REQUIRE( xmipp4::math::erfc(static_cast<float>(sample)) == Catch::Approx(1.0 - xmipp4::math::erf(sample)) );
        REQUIRE( xmipp4::math::erfc(-sample) == Catch::Approx(1.0 - xmipp4::math::erf(-sample)) );
        REQUIRE( xmipp4::math::erfc(-static_cast<float>(sample)) == Catch::Approx(1.0 - xmipp4::math::erf(-sample)) );
    }
}
