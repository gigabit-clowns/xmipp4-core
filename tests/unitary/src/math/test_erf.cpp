// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <xmipp4/core/math/erf.hpp>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE("erf should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(0.1), T(0.112462916018285)},
            {T(0.5), T(0.520499877813047)},
            {T(1.0), T(0.842700792949715)},
            {T(2.0), T(0.995322265018953)},
            {T(10.0), T(1.0)},
        })
    );

    REQUIRE( xmipp4::math::erf(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::erf(-x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE("erfc should produce correct numerical results", "[math]", float, double, long double)
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(1.0)},
            {T(0.1), T(0.887537083982) },
            {T(0.5), T(0.479500122187) },
            {T(1.0), T(0.15729920705) },
            {T(2.0), T(0.004677734981) },
            {T(100.0), T(0.0)},
        })
    );

    REQUIRE( xmipp4::math::erfc(x) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::erfc(-x) == Catch::Approx(T(2.0) - expected) );
}

TEMPLATE_TEST_CASE("erfc and erf should produce complementary results", "[math]", double)
{
    using T = TestType;
    const auto x = GENERATE(T(0.0), T(0.1), T(3.5), T(1.5), T(1.0), T(100.0));
    REQUIRE(xmipp4::math::erfc(x) == Catch::Approx(T(1.0) - xmipp4::math::erf(x)));
}
