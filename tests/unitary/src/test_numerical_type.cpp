// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/numerical_type.hpp>

using namespace xmipp4;

TEST_CASE( "get_size should return the expected size for each type", "[numerical_type]" ) 
{
	numerical_type type;
	std::size_t expected_size;

	std::tie(type, expected_size) = GENERATE(
		table<numerical_type, std::size_t>({
			{ numerical_type::char8, 1UL },
			{ numerical_type::boolean, 1UL },
			{ numerical_type::int8, 1UL },
			{ numerical_type::uint8, 1UL },
			{ numerical_type::int16, 2UL },
			{ numerical_type::uint16, 2UL },
			{ numerical_type::int32, 4UL },
			{ numerical_type::uint32, 4UL },
			{ numerical_type::int64, 8UL },
			{ numerical_type::uint64, 8UL },
			{ numerical_type::float16, 2UL },
			{ numerical_type::float32, 4UL },
			{ numerical_type::float64, 8UL },
			{ numerical_type::complex_float16, 4UL },
			{ numerical_type::complex_float32, 8UL },
			{ numerical_type::complex_float64, 16UL },
		})
	);

	CHECK( get_size(type) == expected_size );
}

TEST_CASE( "get_size of an invalid type should return zero", "[numerical_type]" ) 
{

	const auto invalid_type = GENERATE(
		numerical_type::unknown,
		numerical_type::count
	);

	CHECK( get_size(invalid_type) == 0UL );
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

	CHECK( make_complex(floating_type) == expected_complex_type );
}

TEST_CASE( "make_complex should return unknown for unsupported types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::char8,
		numerical_type::int8,
		numerical_type::uint8,
		numerical_type::int16,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::uint32,
		numerical_type::int64,
		numerical_type::uint64
	);

	CHECK( make_complex(type) == numerical_type::unknown );
}

TEST_CASE( "make_complex should return return itself with complex types", "[numerical_type]" ) 
{
	const auto type = GENERATE(
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	CHECK( make_complex(type) == type );
}

TEST_CASE( "the promote_types of two equal types should be the same as the input types", "[numerical_type]" )
{
	const auto type = GENERATE(
		numerical_type::char8,
		numerical_type::boolean,
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

	CHECK( promote_types(type, type) == type );
}

TEST_CASE( "the promote_types function should be commutative", "[numerical_type]" )
{
	const auto type1 = GENERATE(
		numerical_type::char8,
		numerical_type::boolean,
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
		numerical_type::char8,
		numerical_type::boolean,
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

	CHECK( promote_types(type1, type2) == promote_types(type2, type1) );
}

TEST_CASE( "the promote_types of a floating types is the highest precision type", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::float16, numerical_type::float32, numerical_type::float32 },
			{ numerical_type::float16, numerical_type::float64, numerical_type::float64 },
			{ numerical_type::float32, numerical_type::float64, numerical_type::float64 },
		})
	);

	CHECK( promote_types(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the promote_types of two signed integers is the highest precision signed integer", "[numerical_type]" )
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

	CHECK( promote_types(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the promote_types of two unsigned integers is the highest precision unsigned integer", "[numerical_type]" )
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

	CHECK( promote_types(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the promote_types of a singed and unsigned integer is a signed integer with enough precision", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::char8, numerical_type::int8, numerical_type::int16 },
			{ numerical_type::char8, numerical_type::int16, numerical_type::int16 },
			{ numerical_type::uint8, numerical_type::int16, numerical_type::int16 },
			{ numerical_type::int8, numerical_type::uint16, numerical_type::int32 },
			{ numerical_type::uint8, numerical_type::int32, numerical_type::int32 },
			{ numerical_type::int8, numerical_type::uint32, numerical_type::int64 },
			{ numerical_type::uint8, numerical_type::int64, numerical_type::int64 },
			{ numerical_type::int8, numerical_type::uint64, numerical_type::int64 },
			{ numerical_type::uint16, numerical_type::int32, numerical_type::int32 },
			{ numerical_type::int16, numerical_type::uint32, numerical_type::int64 },
			{ numerical_type::uint16, numerical_type::int64, numerical_type::int64 },
			{ numerical_type::int16, numerical_type::uint64, numerical_type::int64 },
			{ numerical_type::uint32, numerical_type::int64, numerical_type::int64 },
			{ numerical_type::int32, numerical_type::uint64, numerical_type::int64 },
		})
	);

	CHECK( promote_types(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the promote_types of a complex types is the highest precision type", "[numerical_type]" )
{
	numerical_type first_type, second_type, expected_type;
	std::tie(first_type, second_type, expected_type) = GENERATE(
		table<numerical_type, numerical_type, numerical_type>({
			{ numerical_type::complex_float16, numerical_type::complex_float32, numerical_type::complex_float32 },
			{ numerical_type::complex_float16, numerical_type::complex_float64, numerical_type::complex_float64 },
			{ numerical_type::complex_float32, numerical_type::complex_float64, numerical_type::complex_float64 },
		})
	);

	CHECK( promote_types(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the promote_types of a floating point and a complex is the highest precision complex type", "[numerical_type]" )
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

	CHECK( promote_types(first_type, second_type) ==  expected_type );
}

TEST_CASE( "the promote_types of a floating point or complex number and an integer should be the floating point or complex type", "[numerical_type]" )
{
	const auto integer_type = GENERATE(
		numerical_type::boolean,
		numerical_type::char8,
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

	CHECK( promote_types(integer_type, float_or_complex_type) == float_or_complex_type );
}

TEST_CASE( "the promote_types should return numerical_type::unknown for any invalid input", "[numerical_type]" )
{
	const auto first_type = GENERATE(
		numerical_type::unknown,
		numerical_type::count
	);
	const auto second_type = GENERATE(
		numerical_type::unknown,
		numerical_type::boolean,
		numerical_type::char8,
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

	CHECK( promote_types(first_type, second_type) == numerical_type::unknown );
	CHECK( promote_types(second_type, first_type) == numerical_type::unknown );
}

TEST_CASE( "to_string with numerical_type should produce correct results", "[numerical_type]" ) 
{
	numerical_type type;
	std::string expected_str;
	std::tie(type, expected_str) = GENERATE(
		table<numerical_type, std::string>({
			{numerical_type::boolean, "boolean"},
			{numerical_type::char8, "char8"},
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
	CHECK( std::string(to_string(type)) == expected_str );
}
