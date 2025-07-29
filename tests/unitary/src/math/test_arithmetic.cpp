// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <xmipp4/core/math/arithmetic.hpp>

using namespace xmipp4::math;


TEMPLATE_TEST_CASE("multiply_add should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T a, b, c, expected;
    std::tie(a, b, c, expected) = GENERATE(
        table<T, T, T, T>({
            {T(2.31), T(1.35), T(2.21), T(5.3285)},
            {T(-2.5252), T(5.23565), T(73.2121), T(59.99103662)}
        })
    );
    REQUIRE(multiply_add(a, b, c) == Catch::Approx(expected));
}

TEMPLATE_TEST_CASE("mod should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T a, b, expected;
    std::tie(a, b, expected) = GENERATE(
        table<T, T, T>({
            {T(2.31), T(1.35), T(0.96)},
        })
    );
    REQUIRE(mod(a, b) == Catch::Approx(expected));
    REQUIRE(mod(-a, b) == Catch::Approx(-expected));
}
