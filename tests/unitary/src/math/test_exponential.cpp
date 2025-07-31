// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/math/exponential.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <map>
#include <limits>
#include <cstdint>

using namespace xmipp4::math;

TEMPLATE_TEST_CASE( "exp should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(-20.0), T(2.061153622438558e-09)},
            {T(-10.0), T(4.539992976248485e-05)},
            {T(-1.0), T(0.367879441171442)},
            {T(0.0), T(1.0)},
            {T(0.5), T(1.648721270700128)},
            {T(10.0), T(2.202646579480672e+04)},
        })
    );

    REQUIRE( xmipp4::math::exp(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "exp2 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(-20.0), T(9.536743164062500e-07)},
            {T(-10.0), T(9.765625000000000e-04)},
            {T(-1.0), T(0.5)},
            {T(0.0), T(1.0)},
            {T(0.5), T(1.414213562373095)},
            {T(10.0), T(1024.0)},
        })
    );

    REQUIRE( xmipp4::math::exp2(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "exp10 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(-20.0), T(1e-20)},
            {T(-10.0), T(1e-10)},
            {T(-1.0), T(0.1)},
            {T(0.0), T(1.0)},
            {T(0.5), T(3.162277660168380)},
            {T(10.0), T(1e10)},
        })
    );

    REQUIRE( xmipp4::math::exp10(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "expm1 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(1.0), T(1.71828182846)},
            {T(-1.0), T(-0.632120558829)},
            {T(0.5), T(0.6487212707)},
            {T(0.1), T(0.105170918076)},
        })
    );

    REQUIRE( xmipp4::math::expm1(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "log should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.01), T(-4.605170185988091)},
            {T(0.1), T(-2.302585092994046)},
            {T(0.5), T(-0.693147180559945)},
            {T(1.0), T(0.0)},
            {T(5.0), T(1.609437912434100)},
            {T(10.0), T(2.302585092994046)},
            {T(50.0), T(3.912023005428146)},
            {T(1e6), T(13.815510557964274)},
        })
    );

    REQUIRE( xmipp4::math::log(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "log with 0 as input should produce negative infinity", "[math]", float, double, long double ) 
{
    using T = TestType;
    const T value = GENERATE(T(+0.0), T(-0.0));
    REQUIRE( xmipp4::math::log(value) == -std::numeric_limits<T>::infinity() );
}

TEMPLATE_TEST_CASE( "log with negative values as input should produce NaN", "[math]", float, double, long double ) 
{
    using T = TestType;
    const T value = GENERATE(T(-1.0), T(-10.0), T(-100.0));
    REQUIRE( std::isnan(xmipp4::math::log(value)) );
}

TEMPLATE_TEST_CASE( "log2 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.01), T(-6.643856189774724)},
            {T(0.1), T(-3.321928094887362)},
            {T(0.5), T(-1.0)},
            {T(1.0), T(0.0)},
            {T(2.0), T(1.0)},
            {T(5.0), T(2.321928094887362)},
            {T(10.0), T(3.321928094887362)},
            {T(1e6), T(19.931568569324174)},
        })
    );

    REQUIRE( xmipp4::math::log2(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "log2 with 0 as input should produce negative infinity", "[math]", float, double, long double ) 
{
    using T = TestType;
    const T value = GENERATE(T(+0.0), T(-0.0));
    REQUIRE( xmipp4::math::log2(value) == -std::numeric_limits<T>::infinity() );
}

TEMPLATE_TEST_CASE( "log2 with negative values as input should produce NaN", "[math]", float, double, long double ) 
{
    using T = TestType;
    const T value = GENERATE(T(-1.0), T(-10.0), T(-100.0));
    REQUIRE( std::isnan(xmipp4::math::log2(value)) );
}

TEMPLATE_TEST_CASE( "log10 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.01), T(-2.0)},
            {T(0.1), T(-1.0)},
            {T(0.5), T(-0.301029995663981)},
            {T(1.0), T(0.0)},
            {T(2.0), T(0.301029995663981)},
            {T(5.0), T(0.698970004336019)},
            {T(10.0), T(1.0)},
            {T(1e6), T(6.0)},
        })
    );

    REQUIRE( xmipp4::math::log10(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "log10 with 0 as input should produce negative infinity", "[math]", float, double, long double ) 
{
    using T = TestType;
    const T value = GENERATE(T(+0.0), T(-0.0));
    REQUIRE( xmipp4::math::log10(value) == -std::numeric_limits<T>::infinity() );
}

TEMPLATE_TEST_CASE( "log10 with negative values as input should produce NaN", "[math]", float, double, long double ) 
{
    using T = TestType;
    const T value = GENERATE(T(-1.0), T(-10.0), T(-100.0));
    REQUIRE( std::isnan(xmipp4::math::log10(value)) );
}

TEMPLATE_TEST_CASE( "log1p should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(1.0), T(0.69314718056)},
            {T(0.5), T(0.405465108108)},
            {T(10.0), T(2.3978952728)},

        })
    );

    REQUIRE( xmipp4::math::log1p(x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "log1p with -1 as input should produce negative infinity", "[math]", float, double, long double ) 
{
    using T = TestType;
    const auto value = T(-1.0);
    REQUIRE( xmipp4::math::log1p(value) == -std::numeric_limits<T>::infinity() );
}

TEMPLATE_TEST_CASE( "log1p with values less than 1 as input should produce NaN", "[math]", float, double, long double ) 
{
    using T = TestType;
    const T value = GENERATE(T(-1.1), T(-10.0), T(-100.0));
    REQUIRE( std::isnan(xmipp4::math::log1p(value)) );
}

TEMPLATE_TEST_CASE( "log_add should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, y, expected;
    std::tie(x, y, expected) = GENERATE(
        table<T, T, T>({
            {T(0.0), T(0.0), T(0.69314718056)},
            {T(1.0), T(0.0), T(1.31326168752)},
            {T(1.0), T(1.0), T(1.69314718056)},
            {T(1.0), T(-1.0), T(1.12692801104)},
            {T(10.0), T(20.0), T(20.0000453989)},
        })
    );

    REQUIRE( xmipp4::math::log_add(x, y) == Catch::Approx(expected) );
    REQUIRE( xmipp4::math::log_add(y, x) == Catch::Approx(expected) );
}
