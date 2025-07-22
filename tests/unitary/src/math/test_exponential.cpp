// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/math/exponential.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <map>
#include <limits>
#include <cstdint>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE( "exp should produce correct numerical results", "[math]", float, double ) 
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {-20.0, 2.061153622438558e-09},
            {-10.0, 4.539992976248485e-05},
            {-1.0, 0.367879441171442},
            {0.0, 1.0},
            {0.5, 1.648721270700128},
            {10.0, 2.202646579480672e+04},
        })
    );

    REQUIRE( xmipp4::math::exp(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "exp2 should produce correct numerical results", "[math]", float, double ) 
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {-20.0, 9.536743164062500e-07},
            {-10.0, 9.765625000000000e-04},
            {-1.0, 0.5},
            {0.0, 1.0},
            {0.5, 1.414213562373095},
            {10.0, 1024.0},
        })
    );

    REQUIRE( xmipp4::math::exp2(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "exp10 should produce correct numerical results", "[math]", float, double ) 
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {-20.0, 1e-20},
            {-10.0, 1e-10},
            {-1.0, 0.1},
            {0.0, 1.0},
            {0.5, 3.162277660168380},
            {10.0, 1e10},
        })
    );

    REQUIRE( xmipp4::math::exp10(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "log should produce correct numerical results", "[math]", float, double ) 
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.01, -4.605170185988091},
            {0.1, -2.302585092994046},
            {0.5, -0.693147180559945},
            {1.0, 0.0},
            {5.0, 1.609437912434100},
            {10.0, 2.302585092994046},
            {50.0, 3.912023005428146},
            {1e6, 13.815510557964274},
        })
    );

    REQUIRE( xmipp4::math::log(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "log with 0 as input should produce infinity", "[math]", float, double ) 
{
    const TestType value = GENERATE(+0.0, -0.0);
    REQUIRE( xmipp4::math::log(value) == -std::numeric_limits<TestType>::infinity() );
}

TEMPLATE_TEST_CASE( "log with negative values as input should produce NaN", "[math]", float, double ) 
{
    const TestType value = GENERATE(-1.0, -10.0, -100.0);
    REQUIRE( std::isnan(xmipp4::math::log(value)) );
}

TEMPLATE_TEST_CASE( "log2 should produce correct numerical results", "[math]", float, double ) 
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.01, -6.643856189774724},
            {0.1, -3.321928094887362},
            {0.5, -1.0},
            {1.0, 0.0},
            {2.0, 1.0},
            {5.0, 2.321928094887362},
            {10.0, 3.321928094887362},
            {1e6, 19.931568569324174},
        })
    );

    REQUIRE( xmipp4::math::log2(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "log2 with 0 as input should produce infinity", "[math]", float, double ) 
{
    const TestType value = GENERATE(+0.0, -0.0);
    REQUIRE( xmipp4::math::log2(value) == -std::numeric_limits<TestType>::infinity() );
}

TEMPLATE_TEST_CASE( "log2 with negative values as input should produce NaN", "[math]", float, double ) 
{
    const TestType value = GENERATE(-1.0, -10.0, -100.0);
    REQUIRE( std::isnan(xmipp4::math::log2(value)) );
}

TEMPLATE_TEST_CASE( "log10 should produce correct numerical results", "[math]", float, double ) 
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.01, -2.0},
            {0.1, -1.0},
            {0.5, -0.301029995663981},
            {1.0, 0.0},
            {2.0, 0.301029995663981},
            {5.0, 0.698970004336019},
            {10.0, 1.0},
            {1e6, 6.0},
        })
    );

    REQUIRE( xmipp4::math::log10(x) == Catch::Approx(expected) );
}


TEMPLATE_TEST_CASE( "log10 with 0 as input should produce infinity", "[math]", float, double ) 
{
    const TestType value = GENERATE(+0.0, -0.0);
    REQUIRE( xmipp4::math::log10(value) == -std::numeric_limits<TestType>::infinity() );
}

TEMPLATE_TEST_CASE( "log10 with negative values as input should produce NaN", "[math]", float, double ) 
{
    const TestType value = GENERATE(-1.0, -10.0, -100.0);
    REQUIRE( std::isnan(xmipp4::math::log10(value)) );
}














