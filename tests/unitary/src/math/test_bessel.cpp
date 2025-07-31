// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/math/bessel.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <map>
#include <cmath>

using namespace xmipp4::math;


TEMPLATE_TEST_CASE( "cylindrical_bessel_j0 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(1.0)},
            {T(0.1), T(0.997501562066040)},
            {T(0.5), T(0.938469807240813)},
            {T(1.0), T(0.765197686557967)},
            {T(5.0), T(-0.177596771314338)},
            {T(10.0), T(-0.245935764451348)},
            {T(20.0), T(0.167024664340583)},
            {T(50.0), T(0.055812327669252)},
            {T(100.0), T(0.019985850304223)},
        })
    );

    REQUIRE( cylindrical_bessel_j0(x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_j0(-x) == Catch::Approx(expected) );

    REQUIRE( cylindrical_bessel_jn(0, x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_jn(0, -x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_j1 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(0.1), T(0.049937526036242)},
            {T(0.5), T(0.242268457674874)},
            {T(1.0), T(0.440050585744934)},
            {T(5.0), T(-0.327579137591465)},
            {T(10.0), T(0.043472746168861)},
            {T(20.0), T(0.066833124175850)},
            {T(50.0), T(-0.097511828125175)},
            {T(100.0), T(-0.077145352014112)},
        })
    );

    REQUIRE( cylindrical_bessel_j1(x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_j1(-x) == Catch::Approx(-expected) );

    REQUIRE( cylindrical_bessel_jn(1, x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_jn(1, -x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_jn should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    int n;
    T x, expected;
    std::tie(n, x, expected) = GENERATE(
        table<int, T, T>({
            {3, T(0.0), T(0.0)},
            {2, T(1.0), T(0.114903484931901)},
            {4, T(10.0), T(-0.219602686102008)},
            {2, T(0.1), T(0.001248958658800)},
            {10, T(0.5), T(2.613177360822802e-13)},
            {4, T(0.5), T(1.607364763642876e-04)},
            {3, T(1.0), T(0.019563353982668)},
            {3, T(100.0), T(0.076284201720332)},
            {4, T(100.0), T(0.026105809447725)},
            {5, T(100.0), T(-0.074195736964514)},
            {3, T(-100.0), T(-0.076284201720332)},
            {4, T(-100.0), T(0.026105809447725)},
            {5, T(-100.0), T(0.074195736964514)},
            {4, T(-10.0), T(-0.219602686102008)},
            {5, T(-10.0), T(0.234061528186794)},
            {-2, T(1.0), T(0.114903484931901)},
            {-3, T(1.0), T(-0.019563353982668)},
            {-2, T(-1.0), T(0.114903484931901)},
            {-3, T(-1.0), T(0.019563353982668)},
        })
    );

    REQUIRE( cylindrical_bessel_jn(n, x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_y0 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), -std::numeric_limits<T>::infinity()},
            {T(0.1), T(-1.534238651350367)},
            {T(0.5), T(-0.444518733506707)},
            {T(1.0), T(0.088256964215677)},
            {T(5.0), T(-0.308517625249034)},
            {T(10.0), T(0.055671167283599)},
            {T(20.0), T(0.062640596809384)},
            {T(50.0), T(-0.098064995470077)},
            {T(100.0), T(-0.077244313365083)},
        })
    );

    REQUIRE( cylindrical_bessel_y0(x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_yn(0, x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_y' should return NaN for negative values", "[math]", float, double, long double ) 
{
    using T = TestType;
    const auto x = GENERATE(T(-1e-3), T(-1.0), T(-100.0));
    REQUIRE( std::isnan(cylindrical_bessel_y0(x)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(0, x)) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_y1 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            // {T(0.0), -std::numeric_limits<T>::infinity()}, // On Windows returns NaN
            {T(0.1), T(-6.458951094702027)},
            {T(0.5), T(-1.471472392670243)},
            {T(1.0), T(-0.781212821300289)},
            {T(5.0), T(0.147863143391227)},
            {T(10.0), T(0.249015424206954)},
            {T(20.0), T(-0.165511614362521)},
            {T(50.0), T(-0.056795668562015)},
            {T(100.0), T(-0.020372312002760)},
        })
    );

    REQUIRE( cylindrical_bessel_y1(x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_yn(1, x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_y1 should return NaN for negative values", "[math]", float, double, long double ) 
{
    using T = TestType;
    const auto x = GENERATE(T(-1e-3), T(-1.0), T(-100.0));
    REQUIRE( std::isnan(cylindrical_bessel_y1(x)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(1, x)) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_yn should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    int n;
    T x, expected;
    std::tie(n, x, expected) = GENERATE(
        table<int, T, T>({
            // {{2, T(0.0)}, -std::numeric_limits<T>::infinity()}, // On Windows returns NaN
            // {{3, T(0.0)}, -std::numeric_limits<T>::infinity()}, // On Windows returns NaN
            {2, T(1.0), T(-1.650682606816255)},
            {4, T(10.0), T(-0.144949511868094)},
            {2, T(0.1), T(-1.276447832426902e+02)},
            {10, T(0.5), T(-1.219636233495696e+11)},
            {4, T(0.5), T(-4.992725608195124e+02)},
            {3, T(1.0), T(-5.821517605964731)},
            {3, T(100.0), T(0.023445786687761)},
            {4, T(100.0), T(-0.075430119923762)},
            {5, T(100.0), T(-0.029480196281662)},
            {-2, T(1.0), T(-1.650682606816255)},
            {-3, T(1.0), T(5.821517605964731)},
        })
    );

    REQUIRE( cylindrical_bessel_yn(n, x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_yn should return NaN for negative values", "[math]", float, double, long double ) 
{
    using T = TestType;
    const auto x = GENERATE(T(-1e-3), T(-1.0), T(-100.0));
    REQUIRE( std::isnan(cylindrical_bessel_yn(2, x)) );
    REQUIRE( std::isnan(cylindrical_bessel_yn(-3, x)) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_i0 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(1.0)},
            {T(0.1), T(1.002501562934095)},
            {T(0.5), T(1.063483370741324)},
            {T(1.0), T(1.266065877752008)},
            {T(5.0), T(27.239871823604450)},
            {T(10.0), T(2.815716628466255e+03)},
            {T(20.0), T(4.355828255955355e+07)},
            {T(50.0), T(2.932553783849337e+20)},
        })
    );

    REQUIRE( cylindrical_bessel_i0(x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_i0(-x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_in(0, x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_in(0, -x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_i1 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), T(0.0)},
            {T(0.1), T(0.050062526047093)},
            {T(0.5), T(0.257894305390896)},
            {T(1.0), T(0.565159103992485)},
            {T(5.0), T(24.335642142450530)},
            {T(10.0), T(2.670988303701255e+03)},
            {T(20.0), T(4.245497338512778e+07)},
            {T(50.0), T(2.903078590103557e+20)},
        })
    );

    REQUIRE( cylindrical_bessel_i1(x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_i1(-x) == Catch::Approx(-expected) );
    REQUIRE( cylindrical_bessel_in(1, x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_in(1, -x) == Catch::Approx(-expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_in should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    int n;
    T x, expected;
    std::tie(n, x, expected) = GENERATE(
        table<int, T, T>({
            {2, T(0.0), T(0.0)},
            {3, T(0.0), T(0.0)},
            {2, T(1.0), T(0.135747669767038)},
            {2, T(-1.0), T(0.135747669767038)},
            {4, T(10.0), T(1.226490537759491e+03)},
            {4, T(-10.0), T(1.226490537759491e+03)},
            {2, T(0.1), T(0.001251041992242)},
            {10, T(0.5), T(2.643041925881278e-13)},
            {4, T(0.5), T(1.648055498548234e-04)},
            {3, T(1.0),  T(0.022168424924332)},
            {3, T(-1.0),  T(-0.022168424924332)},
            {3, T(5.0), T(10.331150169151138)},
            {3, T(-5.0), T(-10.331150169151138)},
            {4, T(5.0), T(5.108234763642869)},
            {4, T(-5.0), T(5.108234763642869)},
            {5, T(5.0), T(2.157974547322546)},
            {5, T(-5.0), T(-2.157974547322546)},
        })
    );

    REQUIRE( cylindrical_bessel_in(n, x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_in(-n, x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_k0 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), std::numeric_limits<T>::infinity()},
            {T(0.1), T(2.427069024702017)},
            {T(0.5), T(0.924419071227666)},
            {T(1.0), T(0.421024438240708)},
            {T(5.0), T(0.003691098334043)},
            {T(10.0), T(1.778006231616765e-05)},
            {T(50.0), T(3.410167749789496e-23)},
        })
    );

    REQUIRE( cylindrical_bessel_k0(x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_kn(0, x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_k0 should return NaN for negative values", "[math]", float, double, long double ) 
{
    using T = TestType;
    const auto x = GENERATE(T(-1e-3), T(-1.0), T(-100.0));
    REQUIRE( std::isnan(cylindrical_bessel_k0(x)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(0, x)) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_k1 should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    T x, expected;
    std::tie(x, expected) = GENERATE(
        table<T, T>({
            {T(0.0), std::numeric_limits<T>::infinity()},
            {T(0.1), T(9.853844780870606)},
            {T(0.5), T(1.656441120003301)},
            {T(1.0), T(0.601907230197235)},
            {T(5.0), T(0.004044613445452)},
            {T(10.0), T(1.864877345382559e-05)},
            {T(50.0), T(3.444102226717556e-23)},
        })
    );

    REQUIRE( cylindrical_bessel_k1(x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_kn(1, x) == Catch::Approx(expected) );
}   

TEMPLATE_TEST_CASE( "cylindrical_bessel_k1 should return NaN with negative values", "[math]", float, double, long double ) 
{
    using T = TestType;    
    const auto x = GENERATE(T(-1e-3), T(-1.0), T(-100.0));
    REQUIRE( std::isnan(cylindrical_bessel_k1(x)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(1, x)) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_kn should produce correct numerical results", "[math]", float, double, long double ) 
{
    using T = TestType;
    int n;
    T x, expected;
    std::tie(n, x, expected) = GENERATE(
        table<int, T, T>({
            {2, T(0.0), std::numeric_limits<T>::infinity()},
            {3, T(0.0), std::numeric_limits<T>::infinity()},
            {2, T(1.0), T(1.624838898635177)},
            {4, T(10.0), T(3.786143716089198e-05)},
            {2, T(0.1), T(1.995039646421141e+02)},
            {10, T(0.5), T(1.889375693199002e+11)},
            {4, T(0.5), T(7.522450979104038e+02)},
            {3, T(1.0), T(7.101262824737944)},
            {3, T(5.0), T(0.008291768415231)},
            {4, T(5.0), T(0.015259065810501)},
            {5, T(5.0), T(0.032706273712032)},
        })
    );

    REQUIRE( cylindrical_bessel_kn(n, x) == Catch::Approx(expected) );
    REQUIRE( cylindrical_bessel_kn(-n, x) == Catch::Approx(expected) );
}

TEMPLATE_TEST_CASE( "cylindrical_bessel_kn should return NaN with negative values", "[math]", float, double, long double ) 
{
    using T = TestType;
    const auto x = GENERATE(T(-1e-3), T(-1.0), T(-100.0));
    REQUIRE( std::isnan(cylindrical_bessel_kn(2, x)) );
    REQUIRE( std::isnan(cylindrical_bessel_kn(-3, x)) );
}
