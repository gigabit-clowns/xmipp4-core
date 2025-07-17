// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file test_host_numerical_type.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Test for numerical type conversions.
 * @date 2024-11-19
 * 
 */

#include <xmipp4/core/compute/cpu/cpu_numerical_type.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>


using namespace xmipp4;
using namespace xmipp4::compute;

class invalid_numerical_type
{
};

TEST_CASE( "check host numerical types", "[cpu_numerical_type]" )
{
    REQUIRE( cpu_numerical_type<std::int8_t>::value() == numerical_type::int8 );
    REQUIRE( cpu_numerical_type<std::uint8_t>::value() == numerical_type::uint8 );
    REQUIRE( cpu_numerical_type<std::int16_t>::value() == numerical_type::int16 );
    REQUIRE( cpu_numerical_type<std::uint16_t>::value() == numerical_type::uint16 );
    REQUIRE( cpu_numerical_type<std::int32_t>::value() == numerical_type::int32 );
    REQUIRE( cpu_numerical_type<std::uint32_t>::value() == numerical_type::uint32 );
    REQUIRE( cpu_numerical_type<std::int64_t>::value() == numerical_type::int64 );
    REQUIRE( cpu_numerical_type<std::uint64_t>::value() == numerical_type::uint64 );
    REQUIRE( cpu_numerical_type<float16_t>::value() == numerical_type::float16 );
    REQUIRE( cpu_numerical_type<float32_t>::value() == numerical_type::float32 );
    REQUIRE( cpu_numerical_type<float64_t>::value() == numerical_type::float64 );
    REQUIRE( cpu_numerical_type<std::complex<float16_t>>::value() == numerical_type::complex_float16 );
    REQUIRE( cpu_numerical_type<std::complex<float32_t>>::value() == numerical_type::complex_float32 );
    REQUIRE( cpu_numerical_type<std::complex<float64_t>>::value() == numerical_type::complex_float64 );
    REQUIRE( cpu_numerical_type<invalid_numerical_type>::value() == numerical_type::unknown );
}
