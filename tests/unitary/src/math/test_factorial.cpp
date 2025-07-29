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
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
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

TEMPLATE_TEST_CASE( "large_factorial should produce numerically correct results", "[math]", float, double, long double ) 
{
    using T = TestType;
    std::size_t x;
    T expected;
    std::tie(x, expected) = GENERATE(
        table<std::size_t, T>({
            {0U, T(1.0)},
            {1U, T(1.0)},
            {2U, T(2.0)},
            {3U, T(6.0)},
            {4U, T(24.0)},
            {5U, T(120.0)},
            {6U, T(720.0)},
            {7U, T(5040.0)},
            {8U, T(40320.0)},
            {9U, T(362880.0)},
            {10U, T(3628800.0)},
            {12U, T(479001600.0)},
            {13U, T(6227020800.0)},
            {14U, T(87178291200.0)},
            {15U, T(1307674368000.0)},
            {16U, T(20922789888000.0)},
        })
    );

    REQUIRE( large_factorial<T>(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "gamma function should produce numerically correct results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), std::numeric_limits<T>::infinity()},
            {T(1.0), T(1.0)},
            {T(2.0), T(1.0)},
            {T(5.0), T(24.0)},
            {T(10.3), T(716430.68906237720512) },
            {T(-1.5), T(2.363271801207355) },
            {T(-7.6), T(0.00019104791914) },
        })
    );

    REQUIRE( xmipp4::math::gamma(x) == Catch::Approx(expected) );
}
