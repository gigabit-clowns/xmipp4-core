// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <xmipp4/core/math/bspline.hpp>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE("bspline0 should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(1.0)},
            {T(0.25), T(1.0)},
            {T(0.75), T(0.0)},
            {T(1.0), T(0.0)},
            {T(10.0), T(0.0)},
        })
    );
    REQUIRE(bspline0(+x) == Catch::Approx(expected));
    REQUIRE(bspline0(-x) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("bspline1 should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(1.0)},
            {T(0.25), T(0.75)},
            {T(0.5), T(0.5)},
            {T(0.75), T(0.25)},
            {T(1.0), T(0.0)},
            {T(10.0), T(0.0)},
        })
    );
    REQUIRE(bspline1(+x) == Catch::Approx(expected));
    REQUIRE(bspline1(-x) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("bspline2 should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.75)},
            {T(0.25), T(0.6875)},
            {T(0.5), T(0.5)},
            {T(0.75), T(0.28125)},
            {T(1.0), T(0.125)},
            {T(1.25), T(0.03125)},
            {T(1.5), T(0.0)},
            {T(2.0), T(0.0)},
            {T(10.0), T(0.0)},
        })
    );
    REQUIRE(bspline2(+x) == Catch::Approx(expected));
    REQUIRE(bspline2(-x) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("bspline3 should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(2.0)/T(3.0)},
            {T(0.5), T(0.4791666666666666666666)},
            {T(1.0), T(0.1666666666666666666666)},
            {T(1.5), T(0.0208333333333333333333)},
            {T(2.0), T(0.0)},
            {T(2.5), T(0.0)},
            {T(10.0), T(0.0)},
        })
    );
    REQUIRE(bspline3(+x) == Catch::Approx(expected));
    REQUIRE(bspline3(-x) == Catch::Approx(expected));
}
