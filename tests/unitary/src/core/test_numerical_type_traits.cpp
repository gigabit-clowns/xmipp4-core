// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/numerical_type_traits.hpp>

using namespace xmipp4;

TEST_CASE( "numerical_type_of should contain the expected value", "[numerical_type_of]" )
{
    CHECK( numerical_type_of<char>::value == numerical_type::char8 );
    CHECK( numerical_type_of<std::int8_t>::value == numerical_type::int8 );
    CHECK( numerical_type_of<std::uint8_t>::value == numerical_type::uint8 );
    CHECK( numerical_type_of<std::int16_t>::value == numerical_type::int16 );
    CHECK( numerical_type_of<std::uint16_t>::value == numerical_type::uint16 );
    CHECK( numerical_type_of<std::int32_t>::value == numerical_type::int32 );
    CHECK( numerical_type_of<std::uint32_t>::value == numerical_type::uint32 );
    CHECK( numerical_type_of<std::int64_t>::value == numerical_type::int64 );
    CHECK( numerical_type_of<std::uint64_t>::value == numerical_type::uint64 );
    CHECK( numerical_type_of<float16_t>::value == numerical_type::float16 );
    CHECK( numerical_type_of<float32_t>::value == numerical_type::float32 );
    CHECK( numerical_type_of<float64_t>::value == numerical_type::float64 );
    CHECK( numerical_type_of<std::complex<float16_t>>::value == numerical_type::complex_float16 );
    CHECK( numerical_type_of<std::complex<float32_t>>::value == numerical_type::complex_float32 );
    CHECK( numerical_type_of<std::complex<float64_t>>::value == numerical_type::complex_float64 );
}
