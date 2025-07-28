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
    using T = TestType;
    T input, expected;
    std::tie(input, expected) = GENERATE(
        table<T, T>({
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
    using T = TestType;
    T base, exp, expected;
    std::tie(base, exp, expected) = GENERATE(
        table<T, T, T>({
            {T(0.0), T(0.0), T(1.0)},
            {T(1.0), T(0.0), T(1.0)},
            {T(2.0), T(0.0), T(1.0)},
            {T(-1.0), T(0.0), T(1.0)},
            {T(2.0), T(2.0), T(4.0)},
            {T(2.0), T(-2.0), T(0.25)},
            {T(2.0), T(0.5), sqrt2<T>()},
            {T(4.64), T(4.313), T(749.362662244)},
        })
    );

    REQUIRE(xmipp4::math::pow(base, exp) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("sqrt should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T input, expected;
    std::tie(input, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(0.0144), T(0.12)},
            {T(0.01), T(0.1)},
            {T(0.5), sqrt1_2<T>()},
            {T(1.0), T(1.0)},
            {T(2.0), sqrt2<T>()},
            {T(144.0), T(12.0)},
        })
    );

    REQUIRE(xmipp4::math::sqrt(input) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("sqrt should return NaN for negative input", "[math]", float, double, long double)
{
    using T = TestType;
    const auto neg = GENERATE(T(-1e-3), T(-1.0), T(-100.0));
    REQUIRE( std::isnan(xmipp4::math::sqrt(neg)) );
}

TEMPLATE_TEST_CASE("rsqrt should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T input, expected;
    std::tie(input, expected) = GENERATE(
        table<T, T>({
            {T(0.0), std::numeric_limits<T>::infinity()},
            {T(0.0144), T(8.333333333333333333)},
            {T(0.01), T(10.0)},
            {T(0.5), sqrt2<T>()},
            {T(1.0), T(1.0)},
            {T(2.0), sqrt1_2<T>()},
            {T(144.0), T(0.0833333333333333333333)},
        })
    );

    REQUIRE(xmipp4::math::rsqrt(input) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("rsqrt should return NaN for negative input", "[math]", float, double, long double)
{
    using T = TestType;
    const auto neg = GENERATE(T(-1e-3), T(-1.0), T(-100.0));
    REQUIRE( std::isnan(xmipp4::math::rsqrt(neg)) );
}

TEMPLATE_TEST_CASE("cbrt should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T input, expected;
    std::tie(input, expected) = GENERATE(
        table<T, T>({
            {T(0.001728), T(0.12)},
            {T(0.001), T(0.1)},
            {T(1.0), T(1.0)},
            {1e3, 10},
            {1728, T(12.0)},
        })
    );

    REQUIRE(xmipp4::math::cbrt(input) == Catch::Approx(expected));
    REQUIRE(xmipp4::math::cbrt(-input) == Catch::Approx(-expected));
}
