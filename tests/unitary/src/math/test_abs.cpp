// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/math/abs.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <limits>
#include <cstdint>

using namespace xmipp4::math;


TEMPLATE_TEST_CASE( "abs should return the same result when input is positive", "[math]", float, double, int, unsigned int )
{
    TestType input = GENERATE(0, 1, 255, 1000, 123456789);
    REQUIRE( xmipp4::math::abs(input) == input );
}

TEMPLATE_TEST_CASE( "abs should return the same negated result when input is negative", "[math]", float, double, int )
{
    TestType input = GENERATE(0, 1, 255, 1000, 123456789);
    REQUIRE( xmipp4::math::abs(-input) == input );
}

TEMPLATE_TEST_CASE( "abs should return inf when input is plus or minus inf", "[math]", float, double )
{
    const auto input = std::numeric_limits<TestType>::infinity();
    REQUIRE( xmipp4::math::abs(input) == input );
    REQUIRE( xmipp4::math::abs(-input) == input );
}
