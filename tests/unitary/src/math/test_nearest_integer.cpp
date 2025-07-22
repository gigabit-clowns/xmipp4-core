// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <xmipp4/core/math/nearest_integer.hpp>

using namespace xmipp4::math;


TEMPLATE_TEST_CASE("trunc should produce correct numerical results", "[math]", float, double)
{
    TestType input, expected;
    std::tie(input, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {1.0, 1.0},
            {5.24, 5.0},
            {5.5, 5.0},
            {5.78, 5.0},
        })
    );

    REQUIRE(xmipp4::math::trunc(input) == Catch::Approx(expected));
    REQUIRE(xmipp4::math::trunc(-input) == Catch::Approx(-expected));
}

TEMPLATE_TEST_CASE("floor should produce correct numerical results", "[math]", float, double)
{
    TestType input, expected;
    std::tie(input, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {1.0, 1.0},
            {5.24, 5.0},
            {5.5, 5.0},
            {5.78, 5.0},
            {-1.0, -1.0},
            {-5.24, -6.0},
            {-5.5, -6.0},
            {-5.78, -6.0},
        })
    );

    REQUIRE(xmipp4::math::floor(input) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("ceil should produce correct numerical results", "[math]", float, double)
{
    TestType input, expected;
    std::tie(input, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {1.0, 1.0},
            {5.24, 6.0},
            {5.5, 6.0},
            {5.78, 6.0},
            {-1.0, -1.0},
            {-5.24, -5.0},
            {-5.5, -5.0},
            {-5.78, -5.0},
        })
    );

    REQUIRE(xmipp4::math::ceil(input) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("round should produce correct numerical results", "[math]", float, double)
{
    TestType input, expected;
    std::tie(input, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {1.0, 1.0},
            {5.24, 5.0},
            {5.5, 6.0},
            {5.78, 6.0},
        })
    );

    REQUIRE(xmipp4::math::round(input) == Catch::Approx(expected));
    REQUIRE(xmipp4::math::round(-input) == Catch::Approx(-expected));
}
