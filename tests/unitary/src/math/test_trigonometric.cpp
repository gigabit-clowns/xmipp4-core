// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/math/trigonometric.hpp>

#include <xmipp4/core/math/constants.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <limits>
#include <vector>
#include <map>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE("cos produces correct results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 1.0},
            {0.1, 0.99500416527},
            {2.0, -0.41614683654},
            {4.2, -0.49026082134},
            {6.0, 0.96017028665},
            {static_cast<TestType>(2*pi<double>()), 1.0},
            {static_cast<TestType>(pi<double>()), -1.0},
            {static_cast<TestType>(pi_4<double>()), static_cast<TestType>(sqrt1_2<double>())},
        })
    );

    REQUIRE( xmipp4::math::cos(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::cos(-x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE("sin produces correct results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.09983341664},
            {2.0, 0.90929742682},
            {4.2, -0.87157577241},
            {6.0, -0.27941549819},
            {static_cast<TestType>(pi_2<double>()), 1.0},
            {static_cast<TestType>(3*pi_2<double>()), -1.0},
            {static_cast<TestType>(pi_4<double>()), static_cast<TestType>(sqrt1_2<double>())},
        })
    );

    REQUIRE( xmipp4::math::sin(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::sin(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("sincos produces correct results", "[math]", float, double, long double)
{
    TestType x = GENERATE(0.5, 1.35, 133.0, 12.0, 6.53, -6.26);

    const auto sc = xmipp4::math::sincos(x);
    REQUIRE( sc.first == Catch::Approx(sin(x)) );
    REQUIRE( sc.second == Catch::Approx(cos(x)) );
}

TEMPLATE_TEST_CASE("tan produces correct results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.10033467208},
            {2.0, -2.18503986326},
            {4.2, 1.77777977451},
            {6.0, -0.29100619138},
            {static_cast<TestType>(pi_4<double>()), 1.0},
        })
    );

    REQUIRE( xmipp4::math::tan(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::tan(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("acos produces correct results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {-1.0, static_cast<TestType>(pi<double>())},
            {-0.5, 2.0943951},
            {-0.1, 1.67096375},
            {0.0, static_cast<TestType>(pi_2<double>())},
            {0.1, 1.47062891},
            {0.5, 1.04719755},
            {1.0, 0.0},
        })
    );

    REQUIRE( xmipp4::math::acos(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE("acos produces NaN for invalid input", "[math]", float, double, long double)
{
    TestType input = GENERATE(1.1, -1.1);
    REQUIRE(std::isnan(acos(input)));
}

TEMPLATE_TEST_CASE("asin produces correct results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.100167421},
            {0.5, 0.523598776},
            {1.0, static_cast<TestType>(pi_2<double>())},
        })
    );

    REQUIRE( xmipp4::math::asin(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::asin(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("asin produces NaN for invalid input", "[math]", float, double, long double)
{
    TestType input = GENERATE(1.1, -1.1);
    REQUIRE( std::isnan(xmipp4::math::asin(input)) );
}

TEMPLATE_TEST_CASE("atan produces correct results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.0996686525},
            {0.5, 0.463647609},
            {1.0, static_cast<TestType>(pi_4<double>())},
            {5.0, 1.37340077},
            {8.0, 1.44644133},
            {100.0, 1.56079666},
            {std::numeric_limits<TestType>::infinity(), static_cast<TestType>(pi_2<double>())},
        })
    );

    REQUIRE( xmipp4::math::atan(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::atan(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("atan2 produces correct results", "[math]", float, double, long double)
{
    std::pair<TestType, TestType> args;
    TestType expected;
    std::tie(args, expected) = GENERATE(
        table<std::pair<TestType, TestType>, TestType>({
            {{0.0, 0.0}, 0.0},
            {{0.0, 1.0}, 0.0},
            {{1.0, 1.0}, static_cast<TestType>(pi_4<double>())},
            {{1.0, 0.0}, static_cast<TestType>(pi_2<double>())},
            {{1.0, -1.0}, static_cast<TestType>(3*pi_4<double>())},
            {{0.0, -1.0}, static_cast<TestType>(pi<double>())},
        })
    );

    REQUIRE( xmipp4::math::atan2(args.first, args.second) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::atan2(-args.first, args.second) == Catch::Approx(-expected) );
}
    