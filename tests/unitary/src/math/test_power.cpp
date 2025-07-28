// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <xmipp4/core/math/power.hpp>

#include <xmipp4/core/math/constants.hpp>

#include <utility>
#include <map>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE("square should produce correct numerical results", "[math]", float, double, long double, int, unsigned int)
{
    TestType input, expected;
    std::tie(input, expected) = GENERATE(
        table<TestType, TestType>({
            {0, 0},
            {-1, 1},
            {-2, 4},
            {16, 256},
        })
    );

    REQUIRE(xmipp4::math::square(input) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("pow should produce correct numerical results", "[math]", float, double, long double)
{
    TestType base, exp, expected;
    std::tie(base, exp, expected) = GENERATE(
        table<TestType, TestType, TestType>({
            {0.0, 0.0, 1.0},
            {1.0, 0.0, 1.0},
            {2.0, 0.0, 1.0},
            {-1.0, 0.0, 1.0},
            {2.0, 2.0, 4.0},
            {2.0, -2.0, 0.25},
            {2.0, 0.5, static_cast<TestType>(sqrt2<double>())},
            {4.64, 4.313, static_cast<TestType>(749.362662244)},
        })
    );

    REQUIRE(xmipp4::math::pow(base, exp) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("sqrt should produce correct numerical results", "[math]", float, double, long double)
{
    TestType input, expected;
    std::tie(input, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.0144, 0.12},
            {0.01, 0.1},
            {0.5, static_cast<TestType>(sqrt1_2<double>())},
            {1.0, 1.0},
            {2.0, static_cast<TestType>(sqrt2<double>())},
            {144.0, 12.0},
        })
    );

    REQUIRE(xmipp4::math::sqrt(input) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("sqrt should return NaN for negative input", "[math]", float, double, long double)
{
    TestType neg = static_cast<TestType>(-1.0);
    REQUIRE( std::isnan(xmipp4::math::sqrt(neg)) );
}

TEMPLATE_TEST_CASE("rsqrt should produce correct numerical results", "[math]", float, double, long double)
{
    TestType input, expected;
    std::tie(input, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, std::numeric_limits<TestType>::infinity()},
            {0.0144, 8.333333333333333333},
            {0.01, 10.0},
            {0.5, static_cast<TestType>(sqrt2<double>())},
            {1.0, 1.0},
            {2.0, static_cast<TestType>(sqrt1_2<double>())},
            {144.0, 0.0833333333333333333333},
        })
    );

    REQUIRE(xmipp4::math::rsqrt(input) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("rsqrt should return NaN for negative input", "[math]", float, double, long double)
{
    TestType neg = static_cast<TestType>(-1.0);
    REQUIRE( std::isnan(xmipp4::math::rsqrt(neg)) );
}

TEMPLATE_TEST_CASE("cbrt should produce correct numerical results", "[math]", float, double, long double)
{
    TestType input, expected;
    std::tie(input, expected) = GENERATE(
        table<TestType, TestType>({
            {0.001728, 0.12},
            {0.001, 0.1},
            {1.0, 1.0},
            {1e3, 10},
            {1728, 12.0},
        })
    );

    REQUIRE(xmipp4::math::cbrt(input) == Catch::Approx(expected));
    REQUIRE(xmipp4::math::cbrt(-input) == Catch::Approx(-expected));
}
