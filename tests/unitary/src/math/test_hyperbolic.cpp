// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/math/hyperbolic.hpp>

#include <xmipp4/core/math/constants.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <limits>
#include <cmath>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE("cosh produces correct results", "[math]", float, double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 1.0},
            {0.1, 1.00500416806},
            {2.0, 3.76219569108},
            {4.2, 33.3506633089},
            {6.0, 201.715636122},
        })
    );

    REQUIRE(cosh(x) == Catch::Approx(expected));
    REQUIRE(cosh(-x) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("sinh produces correct results", "[math]", float, double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.10016675002},
            {2.0, 3.62686040785},
            {4.2, 33.3356677321},
            {6.0, 201.71315737},
        })
    );

    REQUIRE(sinh(x) == Catch::Approx(expected));
    REQUIRE(sinh(-x) == Catch::Approx(-expected));
}

TEMPLATE_TEST_CASE("tanh produces correct results", "[math]", float, double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.09966799462},
            {2.0, 0.96402758007},
            {4.2, 0.99955036646},
            {6.0, 0.99998771165},
        })
    );

    REQUIRE(tanh(x) == Catch::Approx(expected));
    REQUIRE(tanh(-x) == Catch::Approx(-expected));
}

TEMPLATE_TEST_CASE("acosh produces correct results", "[math]", float, double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {1.0, 0.0},
            {1.5, 0.962423650119207},
            {10.0, 2.993222846126381},
        })
    );

    REQUIRE(acosh(x) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("acosh produces NaN for invalid input", "[math]", float, double)
{
    const TestType input = GENERATE(0.5, -1.0);
    REQUIRE( std::isnan(acosh(input)) );
}

TEMPLATE_TEST_CASE("asinh produces correct results", "[math]", float, double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.099834078899208},
            {1.0, 0.881373587019543},
            {10.0, 2.998222950297970},
            {100.0, 5.298342365610589},
        })
    );

    REQUIRE(asinh(x) == Catch::Approx(expected));
    REQUIRE(asinh(-x) == Catch::Approx(-expected));
}

TEMPLATE_TEST_CASE("atanh produces correct results", "[math]", float, double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.100335347731076},
            {0.5, 0.549306144334055},
            {0.9, 1.472219489583220},
            {1.0, std::numeric_limits<TestType>::infinity()},
        })
    );
    REQUIRE(atanh(x) == Catch::Approx(expected));
    REQUIRE(atanh(-x) == Catch::Approx(-expected));
}

TEMPLATE_TEST_CASE("atanh produces NaN for invalid input", "[math]", float, double)
{
    const TestType input = GENERATE(1.1, -1.1);
    REQUIRE( std::isnan(atanh(input)) );
}
