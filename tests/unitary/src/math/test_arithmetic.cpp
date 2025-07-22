// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <xmipp4/core/math/arithmetic.hpp>

using namespace xmipp4::math;


TEMPLATE_TEST_CASE("multiply_add should produce correct numerical results", "[math]", float, double, long double)
{
    TestType a, b, c, expected;
    std::tie(a, b, c, expected) = GENERATE(
        table<TestType, TestType, TestType, TestType>({
            {2.31, 1.35, 2.21, 5.3285},
            {-2.5252, 5.23565, 73.2121, 59.99103662}
        })
    );
    REQUIRE(multiply_add(a, b, c) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("mod should produce correct numerical results", "[math]", float, double, long double)
{
    TestType a, b, expected;
    std::tie(a, b, expected) = GENERATE(
        table<TestType, TestType, TestType>({
            {2.31, 1.35, 0.96},
        })
    );
    REQUIRE(mod(a, b) == Catch::Approx(expected));
    REQUIRE(mod(-a, b) == Catch::Approx(-expected));
}
