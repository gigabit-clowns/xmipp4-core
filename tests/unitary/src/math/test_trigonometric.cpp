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
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(1.0)},
            {T(0.1), T(0.99500416527)},
            {T(2.0), T(-0.41614683654)},
            {T(4.2), T(-0.49026082134)},
            {T(6.0), T(0.96017028665)},
            {2*pi<T>(), T(1.0)},
            {pi<T>(), T(-1.0)},
            {pi_4<T>(), sqrt1_2<T>()},
        })
    );

    REQUIRE( xmipp4::math::cos(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::cos(-x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE("sin produces correct results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(0.1), T(0.09983341664)},
            {T(2.0), T(0.90929742682)},
            {T(4.2), T(-0.87157577241)},
            {T(6.0), T(-0.27941549819)},
            {pi_2<T>(), T(1.0)},
            {3*pi_2<T>(), T(-1.0)},
            {pi_4<T>(), sqrt1_2<T>()},
        })
    );

    REQUIRE( xmipp4::math::sin(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::sin(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("sincos produces correct results", "[math]", float, double, long double)
{
    using T = TestType;
    T x = GENERATE(T(0.5), T(1.35), T(133.0), T(12.0), T(6.53), T(-6.26));

    const auto sc = xmipp4::math::sincos(x);
    REQUIRE( sc.first == Catch::Approx(sin(x)) );
    REQUIRE( sc.second == Catch::Approx(cos(x)) );
}

TEMPLATE_TEST_CASE("tan produces correct results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(0.1), T(0.10033467208)},
            {T(2.0), T(-2.18503986326)},
            {T(4.2), T(1.77777977451)},
            {T(6.0), T(-0.29100619138)},
            {pi_4<T>(), T(1.0)},
        })
    );

    REQUIRE( xmipp4::math::tan(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::tan(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("acos produces correct results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(-1.0), pi<T>()},
            {T(-0.5), T(2.0943951)},
            {T(-0.1), T(1.67096375)},
            {T(0.0), pi_2<T>()},
            {T(0.1), T(1.47062891)},
            {T(0.5), T(1.04719755)},
            {T(1.0), T(0.0)},
        })
    );

    REQUIRE( xmipp4::math::acos(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE("acos produces NaN for invalid input", "[math]", float, double, long double)
{
    using T = TestType;
    T input = GENERATE(T(1.1), T(-1.1));
    REQUIRE(std::isnan(acos(input)));
}

TEMPLATE_TEST_CASE("asin produces correct results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(0.1), T(0.100167421)},
            {T(0.5), T(0.523598776)},
            {T(1.0), pi_2<T>()},
        })
    );

    REQUIRE( xmipp4::math::asin(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::asin(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("asin produces NaN for invalid input", "[math]", float, double, long double)
{
    using T = TestType;
    T input = GENERATE(T(1.1), T(-1.1));
    REQUIRE( std::isnan(xmipp4::math::asin(input)) );
}

TEMPLATE_TEST_CASE("atan produces correct results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(0.1), T(0.0996686525)},
            {T(0.5), T(0.463647609)},
            {T(1.0), pi_4<T>()},
            {T(5.0), T(1.37340077)},
            {T(8.0), T(1.44644133)},
            {T(100.0), T(1.56079666)},
            {std::numeric_limits<T>::infinity(), pi_2<T>()},
        })
    );

    REQUIRE( xmipp4::math::atan(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::atan(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("atan2 produces correct results", "[math]", float, double, long double)
{
    using T = TestType;
    std::pair<T, T> args;
    T expected;
    std::tie(args, expected) = GENERATE(
        table<std::pair<T, T>, T>({
            {{T(0.0), T(0.0)}, T(0.0)},
            {{T(0.0), T(1.0)}, T(0.0)},
            {{T(1.0), T(1.0)}, pi_4<T>()},
            {{T(1.0), T(0.0)}, pi_2<T>()},
            {{T(1.0), T(-1.0)}, 3*pi_4<T>()},
            {{T(0.0), T(-1.0)}, pi<T>()},
        })
    );

    REQUIRE( xmipp4::math::atan2(args.first, args.second) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::atan2(-args.first, args.second) == Catch::Approx(-expected) );
}
    