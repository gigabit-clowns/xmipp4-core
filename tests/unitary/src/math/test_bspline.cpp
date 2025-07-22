// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <xmipp4/core/math/bspline.hpp>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE("bspline0 should produce correct numerical results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 1.0},
            {0.25, 1.0},
            {0.75, 0.0},
            {1.0, 0.0},
            {10.0, 0.0},
        })
    );
    REQUIRE(bspline0(+x) == Catch::Approx(expected));
    REQUIRE(bspline0(-x) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("bspline1 should produce correct numerical results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 1.0},
            {0.25, 0.75},
            {0.5, 0.5},
            {0.75, 0.25},
            {1.0, 0.0},
            {10.0, 0.0},
        })
    );
    REQUIRE(bspline1(+x) == Catch::Approx(expected));
    REQUIRE(bspline1(-x) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("bspline2 should produce correct numerical results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.75},
            {0.25, 0.6875},
            {0.5, 0.5},
            {0.75, 0.28125},
            {1.0, 0.125},
            {1.25, 0.03125},
            {1.5, 0.0},
            {2.0, 0.0},
            {10.0, 0.0},
        })
    );
    REQUIRE(bspline2(+x) == Catch::Approx(expected));
    REQUIRE(bspline2(-x) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("bspline3 should produce correct numerical results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 2.0/3.0},
            {0.5, 0.4791666666666666666666},
            {1.0, 0.1666666666666666666666},
            {1.5, 0.0208333333333333333333},
            {2.0, 0.0},
            {2.5, 0.0},
            {10.0, 0.0},
        })
    );
    REQUIRE(bspline3(+x) == Catch::Approx(expected));
    REQUIRE(bspline3(-x) == Catch::Approx(expected));
}
