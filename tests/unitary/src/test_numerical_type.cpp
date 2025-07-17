// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/numerical_type.hpp>

using namespace xmipp4::compute;


TEST_CASE( "to_string with numerical_type should produce correct results", "[numerical_type]" ) 
{
    REQUIRE( std::string(to_string(numerical_type::unknown)) == "unknown" ); 
    REQUIRE( std::string(to_string(numerical_type::int8)) == "int8" );
    REQUIRE( std::string(to_string(numerical_type::uint8)) == "uint8" );
    REQUIRE( std::string(to_string(numerical_type::int16)) == "int16" );
    REQUIRE( std::string(to_string(numerical_type::uint16)) == "uint16" );
    REQUIRE( std::string(to_string(numerical_type::int32)) == "int32" );
    REQUIRE( std::string(to_string(numerical_type::uint32)) == "uint32" );
    REQUIRE( std::string(to_string(numerical_type::int64)) == "int64" );
    REQUIRE( std::string(to_string(numerical_type::uint64)) == "uint64" );
    REQUIRE( std::string(to_string(numerical_type::float16)) == "float16" );
    REQUIRE( std::string(to_string(numerical_type::brain_float16)) == "brain_float16" );
    REQUIRE( std::string(to_string(numerical_type::float32)) == "float32" );
    REQUIRE( std::string(to_string(numerical_type::float64)) == "float64" );
    REQUIRE( std::string(to_string(numerical_type::complex_float16)) == "complex_float16" );
    REQUIRE( std::string(to_string(numerical_type::complex_float32)) == "complex_float32" );
    REQUIRE( std::string(to_string(numerical_type::complex_float64)) == "complex_float64" );
}

TEST_CASE( "from_string with numerical_type should produce correct results", "[numerical_type]" ) 
{
    numerical_type type;

    REQUIRE( from_string("unknown", type) );
    REQUIRE( type == numerical_type::unknown );
    REQUIRE( from_string("int8", type) );
    REQUIRE( type == numerical_type::int8 );
    REQUIRE( from_string("uint8", type) );
    REQUIRE( type == numerical_type::uint8 );
    REQUIRE( from_string("int16", type) );
    REQUIRE( type == numerical_type::int16 );
    REQUIRE( from_string("uint16", type) );
    REQUIRE( type == numerical_type::uint16 );
    REQUIRE( from_string("int32", type) );
    REQUIRE( type == numerical_type::int32 );
    REQUIRE( from_string("uint32", type) );
    REQUIRE( type == numerical_type::uint32 );
    REQUIRE( from_string("int64", type) );
    REQUIRE( type == numerical_type::int64 );
    REQUIRE( from_string("uint64", type) );
    REQUIRE( type == numerical_type::uint64 );
    REQUIRE( from_string("float16", type) );
    REQUIRE( type == numerical_type::float16 );
    REQUIRE( from_string("brain_float16", type) );
    REQUIRE( type == numerical_type::brain_float16 );
    REQUIRE( from_string("float32", type) );
    REQUIRE( type == numerical_type::float32 );
    REQUIRE( from_string("float64", type) );
    REQUIRE( type == numerical_type::float64 );
    REQUIRE( from_string("complex_float16", type) );
    REQUIRE( type == numerical_type::complex_float16 );
    REQUIRE( from_string("complex_float32", type) );
    REQUIRE( type == numerical_type::complex_float32 );
    REQUIRE( from_string("complex_float64", type) );
    REQUIRE( type == numerical_type::complex_float64 );
}
