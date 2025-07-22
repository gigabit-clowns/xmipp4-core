// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/erf.hpp>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE("erf should produce correct numerical results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 0.0},
            {0.1, 0.112462916018285},
            {0.5, 0.520499877813047},
            {1.0, 0.842700792949715},
            {2.0, 0.995322265018953},
            {10.0, 1.0},
        })
    );

    REQUIRE( xmipp4::math::erf(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::erf(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("erfc should produce correct numerical results", "[math]", float, double, long double)
{
    TestType x, expected;
    std::tie(x, expected) = GENERATE(
        table<TestType, TestType>({
            {0.0, 1.0},
            {0.1, 0.887537083982 },
            {0.5, 0.479500122187 },
            {1.0, 0.15729920705 },
            {2.0, 0.004677734981 },
            {100.0, 0.0},
        })
    );

    REQUIRE( xmipp4::math::erfc(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::erfc(-x) == Catch::Approx(2.0 - expected) );
}

TEMPLATE_TEST_CASE("erfc and erf should produce complementary results", "[math]", double)

{
    const TestType x = GENERATE(0.0, 0.1, 3.5, 1.5, 1.0, 100.0);
    REQUIRE(xmipp4::math::erfc(x) == Catch::Approx(1.0 - xmipp4::math::erf(x)));
}
