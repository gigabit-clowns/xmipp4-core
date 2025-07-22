// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/math/factorial.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <map>
#include <limits>
#include <cstdint>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE( "factorial should produce expected values for small inputs", "[math]", std::uint16_t, std::uint32_t, std::uint64_t ) 
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0, 1},
            {1, 1},
            {2, 2},
            {3, 6},
            {4, 24},
            {5, 120},
            {6, 720},
            {7, 5040},
            {8, 40320},
        })
    );

    REQUIRE( factorial(x) == expected);
}

TEST_CASE( "factorial function should overflow when input produces out of range results", "[math]" ) 
{
    REQUIRE( factorial(uint8_t(6)) == 208 );
}

TEMPLATE_TEST_CASE( "large_factorial should produce numerically correct results", "[math]", float, double ) 
{
    std::size_t x;
    TestType expected;
    std::tie(x, expected) = GENERATE(
        table<std::size_t, TestType>({
            {0U, 1.0},
            {1U, 1.0},
            {2U, 2.0},
            {3U, 6.0},
            {4U, 24.0},
            {5U, 120.0},
            {6U, 720.0},
            {7U, 5040.0},
            {8U, 40320.0},
            {9U, 362880.0},
            {10U, 3628800.0},
            {12U, 479001600.0},
            {13U, 6227020800.0},
            {14U, 87178291200.0},
            {15U, 1307674368000.0},
            {16U, 20922789888000.0},
        })
    );

    REQUIRE( large_factorial<TestType>(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "gamma function should produce numerically correct results", "[math]", float, double ) 
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, std::numeric_limits<TestType>::infinity()},
            {1.0, 1.0},
            {2.0, 1.0},
            {5.0, 24.0},
            {10.3, 716430.68906237720512 },
            {-1.5, 2.363271801207355 },
            {-7.6, 0.00019104791914 },
        })
    );

    REQUIRE( xmipp4::math::gamma(x) == Catch::Approx(expected) );
}
