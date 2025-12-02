// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/numerical_type.hpp>

using namespace xmipp4;

TEST_CASE( "numerical_type_of should contain the expected value", "[numerical_type_of]" )
{
    REQUIRE( numerical_type_of<std::int8_t>::value == numerical_type::int8 );
    REQUIRE( numerical_type_of<std::uint8_t>::value == numerical_type::uint8 );
    REQUIRE( numerical_type_of<std::int16_t>::value == numerical_type::int16 );
    REQUIRE( numerical_type_of<std::uint16_t>::value == numerical_type::uint16 );
    REQUIRE( numerical_type_of<std::int32_t>::value == numerical_type::int32 );
    REQUIRE( numerical_type_of<std::uint32_t>::value == numerical_type::uint32 );
    REQUIRE( numerical_type_of<std::int64_t>::value == numerical_type::int64 );
    REQUIRE( numerical_type_of<std::uint64_t>::value == numerical_type::uint64 );
    REQUIRE( numerical_type_of<float16_t>::value == numerical_type::float16 );
    REQUIRE( numerical_type_of<float32_t>::value == numerical_type::float32 );
    REQUIRE( numerical_type_of<float64_t>::value == numerical_type::float64 );
    REQUIRE( numerical_type_of<std::complex<float16_t>>::value == numerical_type::complex_float16 );
    REQUIRE( numerical_type_of<std::complex<float32_t>>::value == numerical_type::complex_float32 );
    REQUIRE( numerical_type_of<std::complex<float64_t>>::value == numerical_type::complex_float64 );
}

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
			{numerical_type::float32, "float32"},
			{numerical_type::float64, "float64"},
			{numerical_type::complex_float16, "complex_float16"},
			{numerical_type::complex_float32, "complex_float32"},
			{numerical_type::complex_float64, "complex_float64"}
		})
	);
	REQUIRE( std::string(to_string(type)) == expected_str );
}

TEST_CASE( "is_unsigned should return true with unsigned numerical_types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::uint8,
		numerical_type::uint16,
		numerical_type::uint32,
		numerical_type::uint64
	);

	REQUIRE( is_unsigned(type) == true );
}

TEST_CASE( "is_unsigned should return false with signed numerical_types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::int16,
		numerical_type::int32,
		numerical_type::int64,
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	REQUIRE( is_unsigned(type) == false );
}

TEST_CASE( "is_integer should return true with integral numerical_types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64
	);

	REQUIRE( is_integer(type) == true );
}

TEST_CASE( "is_integer should return false with non integral numerical_types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	REQUIRE( is_integer(type) == false );
}

TEST_CASE( "is_float should return true with floating point numerical_types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64
	);

	REQUIRE( is_float(type) == true );
}

TEST_CASE( "is_float should return false with non floating point numerical_types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64,
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	REQUIRE( is_float(type) == false );
}

TEST_CASE( "is_complex should return true with complex numerical_types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	REQUIRE( is_complex(type) == true );
}

TEST_CASE( "is_complex should return false with non complex numerical_types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64,
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64
	);

	REQUIRE( is_complex(type) == false );
}

TEST_CASE( "make_complex should return the equivalent complex type for supported floating point types", "[numerical_type]" ) 
{
	numerical_type floating_type;
	numerical_type expected_complex_type;

	std::tie(floating_type, expected_complex_type) = GENERATE(
		table<numerical_type, numerical_type>({
			{numerical_type::float16, numerical_type::complex_float16},
			{numerical_type::float32, numerical_type::complex_float32},
			{numerical_type::float64, numerical_type::complex_float64}
		})
	);

	REQUIRE( make_complex(floating_type) == expected_complex_type );
}

TEST_CASE( "make_complex should return unknown for unsupported types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64
	);

	REQUIRE( make_complex(type) == numerical_type::unknown );
}

TEST_CASE( "make_complex should return return itself with complex types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	REQUIRE( make_complex(type) == type );
}

TEST_CASE( "the common_type of two equal types should be the same as the input types", "[numerical_type]" )
{
	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64,
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	REQUIRE( common_type(type, type) == type );
}

TEST_CASE( "the common_type function should be commutative", "[numerical_type]" )
{
	const auto type1 = GENERATE(
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64,
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	const auto type2 = GENERATE(
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64,
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	REQUIRE( common_type(type1, type2) == common_type(type2, type1) );
}

TEST_CASE( "the common_type of a floating types is the highest precision type", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::float16, numerical_type::float32, numerical_type::float32 },
			{ numerical_type::float16, numerical_type::float64, numerical_type::float64 },
			{ numerical_type::float32, numerical_type::float64, numerical_type::float64 },
		})
	);

	REQUIRE( common_type(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the common_type of two signed integers is the highest precision signed integer", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::int8, numerical_type::int16, numerical_type::int16 },
			{ numerical_type::int8, numerical_type::int32, numerical_type::int32 },
			{ numerical_type::int8, numerical_type::int64, numerical_type::int64 },
			{ numerical_type::int16, numerical_type::int32, numerical_type::int32 },
			{ numerical_type::int16, numerical_type::int64, numerical_type::int64 },
			{ numerical_type::int32, numerical_type::int64, numerical_type::int64 },
		})
	);

	REQUIRE( common_type(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the common_type of two unsigned integers is the highest precision unsigned integer", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::uint8, numerical_type::uint16, numerical_type::uint16 },
			{ numerical_type::uint8, numerical_type::uint32, numerical_type::uint32 },
			{ numerical_type::uint8, numerical_type::uint64, numerical_type::uint64 },
			{ numerical_type::uint16, numerical_type::uint32, numerical_type::uint32 },
			{ numerical_type::uint16, numerical_type::uint64, numerical_type::uint64 },
			{ numerical_type::uint32, numerical_type::uint64, numerical_type::uint64 },
		})
	);

	REQUIRE( common_type(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the common_type of a singed and unsigned integer is the highest precision signed integer", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::uint8, numerical_type::int16, numerical_type::int16 },
			{ numerical_type::int8, numerical_type::uint16, numerical_type::int16 },
			{ numerical_type::uint8, numerical_type::int32, numerical_type::int32 },
			{ numerical_type::int8, numerical_type::uint32, numerical_type::int32 },
			{ numerical_type::uint8, numerical_type::int64, numerical_type::int64 },
			{ numerical_type::int8, numerical_type::uint64, numerical_type::int64 },
			{ numerical_type::uint16, numerical_type::int32, numerical_type::int32 },
			{ numerical_type::int16, numerical_type::uint32, numerical_type::int32 },
			{ numerical_type::uint16, numerical_type::int64, numerical_type::int64 },
			{ numerical_type::int16, numerical_type::uint64, numerical_type::int64 },
			{ numerical_type::uint32, numerical_type::int64, numerical_type::int64 },
			{ numerical_type::int32, numerical_type::uint64, numerical_type::int64 },
		})
	);

	REQUIRE( common_type(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the common_type of a complex types is the highest precision type", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::complex_float16, numerical_type::complex_float32, numerical_type::complex_float32 },
			{ numerical_type::complex_float16, numerical_type::complex_float64, numerical_type::complex_float64 },
			{ numerical_type::complex_float32, numerical_type::complex_float64, numerical_type::complex_float64 },
		})
	);

	REQUIRE( common_type(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the common_type of a floating point and a complex is the highest precision complex type", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::float16, numerical_type::complex_float16, numerical_type::complex_float16 },
			{ numerical_type::float16, numerical_type::complex_float32, numerical_type::complex_float32 },
			{ numerical_type::float16, numerical_type::complex_float64, numerical_type::complex_float64 },
			{ numerical_type::float32, numerical_type::complex_float16, numerical_type::complex_float32 },
			{ numerical_type::float32, numerical_type::complex_float32, numerical_type::complex_float32 },
			{ numerical_type::float32, numerical_type::complex_float64, numerical_type::complex_float64 },
			{ numerical_type::float64, numerical_type::complex_float16, numerical_type::complex_float64 },
			{ numerical_type::float64, numerical_type::complex_float32, numerical_type::complex_float64 },
			{ numerical_type::float64, numerical_type::complex_float64, numerical_type::complex_float64 },
		})
	);

	REQUIRE( common_type(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the common_type of a floating point or complex number and an integer should be the floating point or complex type", "[numerical_type]" )
{
	const auto integer_type = GENERATE(
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64
	);

	const auto float_or_complex_type = GENERATE(
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	REQUIRE( common_type(integer_type, float_or_complex_type) == float_or_complex_type );
}

TEST_CASE( "the common_type should return numerical_type::unknown for any invalid input", "[numerical_type]" )
{
	const auto first_type = GENERATE(
		numerical_type::unknown,
		numerical_type::count
	);
	const auto second_type = GENERATE(
		numerical_type::unknown,
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64,
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64,
		numerical_type::count
	);

	REQUIRE( common_type(first_type, second_type) == numerical_type::unknown );
	REQUIRE( common_type(second_type, first_type) == numerical_type::unknown );
}
