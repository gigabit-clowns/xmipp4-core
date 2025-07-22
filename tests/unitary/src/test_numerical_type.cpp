// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/numerical_type.hpp>

using namespace xmipp4;

TEST_CASE( "to_string with numerical_type should produce correct results", "[numerical_type]" ) 
{
    numerical_type type;
    std::string expected_str;
    std::tie(type, expected_str) = GENERATE(
        table<numerical_type, std::string>({
            {numerical_type::unknown, "unknown"},
            {numerical_type::int8, "int8"},
            {numerical_type::uint8, "uint8"},
            {numerical_type::int16, "int16"},
            {numerical_type::uint16, "uint16"},
            {numerical_type::int32, "int32"},
            {numerical_type::uint32, "uint32"},
            {numerical_type::int64, "int64"},
            {numerical_type::uint64, "uint64"},
            {numerical_type::float16, "float16"},
            {numerical_type::brain_float16, "brain_float16"},
            {numerical_type::float32, "float32"},
            {numerical_type::float64, "float64"},
            {numerical_type::complex_float16, "complex_float16"},
            {numerical_type::complex_float32, "complex_float32"},
            {numerical_type::complex_float64, "complex_float64"}
        })
    );
    REQUIRE( std::string(to_string(type)) == expected_str );
}

TEST_CASE( "from_string with numerical_type should produce correct results", "[numerical_type]" ) 
{
    std::string input_str;
    bool expected_valid;
    numerical_type expected_type;
    std::tie(input_str, expected_valid, expected_type) = GENERATE(
        table<std::string, bool, numerical_type>({
            {"unknown", true, numerical_type::unknown},
            {"int8", true, numerical_type::int8},
            {"uint8", true, numerical_type::uint8},
            {"int16", true, numerical_type::int16},
            {"uint16", true, numerical_type::uint16},
            {"int32", true, numerical_type::int32},
            {"uint32", true, numerical_type::uint32},
            {"int64", true, numerical_type::int64},
            {"uint64", true, numerical_type::uint64},
            {"float16", true, numerical_type::float16},
            {"brain_float16", true, numerical_type::brain_float16},
            {"float32", true, numerical_type::float32},
            {"float64", true, numerical_type::float64},
            {"complex_float16", true, numerical_type::complex_float16},
            {"complex_float32", true, numerical_type::complex_float32},
            {"complex_float64", true, numerical_type::complex_float64},
            {"invalid", false, numerical_type::unknown} // el valor no importa si es inválido
        })
    );

    numerical_type type;
    bool valid = from_string(input_str, type);
    REQUIRE(valid == expected_valid);
    if (expected_valid)
    {
        REQUIRE(type == expected_type);
    }
}
