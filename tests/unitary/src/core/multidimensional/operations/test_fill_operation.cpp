// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operations/fill_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("get_name in fill_operation should return fill", "[fill_operation]")
{
	std::int8_t x = 2;
	fill_operation op(x);
	CHECK( op.get_name() == "fill" );
}

TEST_CASE("serialize_parameters should return the same representation for equal fill values", "[fill_operation]")
{
	std::int16_t value11 = 1;
	std::int16_t value12 = 1;
	float64_t value21 = 3.4;
	float64_t value22 = 3.4;
	std::complex<float32_t> value31(2.0f, 24.0f);
	std::complex<float32_t> value32(2.0f, 24.0f);

	CHECK( fill_operation(value11).serialize_parameters() == fill_operation(value12).serialize_parameters() );
	CHECK( fill_operation(value21).serialize_parameters() == fill_operation(value22).serialize_parameters() );
	CHECK( fill_operation(value31).serialize_parameters() == fill_operation(value32).serialize_parameters() );
}

TEST_CASE("serialize_parameters should return the unequal representation for unequal fill values", "[fill_operation]")
{
	std::int16_t value11 = 1;
	std::int16_t value12 = 2;
	float64_t value21 = 3.4;
	float64_t value22 = 3.3;
	std::complex<float32_t> value31(2.0f, 23.0f);
	std::complex<float32_t> value32(2.0f, 24.0f);

	CHECK( fill_operation(value11).serialize_parameters() != fill_operation(value12).serialize_parameters() );
	CHECK( fill_operation(value21).serialize_parameters() != fill_operation(value22).serialize_parameters() );
	CHECK( fill_operation(value31).serialize_parameters() != fill_operation(value32).serialize_parameters() );
}

TEST_CASE("sanitize_operands in fill_operation should not modify output descriptor", "[fill_operation]")
{
	const auto extents = GENERATE(
		std::vector<std::size_t>{1, 2, 4},
		std::vector<std::size_t>{6, 3}
	);
	const auto data_type = GENERATE(
		numerical_type::int8,
		numerical_type::complex_float32
	);
	const array_descriptor descriptor(
		strided_layout::make_contiguous_layout(make_span(extents)),
		data_type
	);

	std::array<array_descriptor, 1> output_descriptors = {descriptor};

	std::int8_t x = 0;
	fill_operation op(x);
	op.sanitize_operands(
		make_span(output_descriptors), 
		{}
	);

	CHECK( output_descriptors[0] == descriptor );
}

TEST_CASE("sanitize_operands in fill_operation should throw if an invalid amount of inputs is provided", "[fill_operation]")
{
	const auto n_input = GENERATE(1, 2, 20);

	std::array<array_descriptor, 1> output_descriptors = {};
	std::vector<array_descriptor> input_descriptors(n_input);

	std::int8_t x = 0;
	fill_operation op(x);
	REQUIRE_THROWS_MATCHES(
		op.sanitize_operands(
			make_span(output_descriptors), 
			make_span(input_descriptors)
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"fill_operation requires exactly no input operand."
		)
	);
}

TEST_CASE("sanitize_operands in fill_operation should throw if an invalid amount of outputs is provided", "[fill_operation]")
{
	const auto n_outputs = GENERATE(0, 2, 20);

	std::vector<array_descriptor> output_descriptors(n_outputs);

	std::int8_t x = 0;
	fill_operation op(x);
	REQUIRE_THROWS_MATCHES(
		op.sanitize_operands(
			make_span(output_descriptors), 
			{}
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"fill_operation requires exactly one output operand."
		)
	);
}

TEST_CASE("sanitize_operands in fill_operation should throw if output is not initialized", "[fill_operation]")
{
	std::array<array_descriptor, 1> output_descriptors = {};

	std::int8_t x = 0;
	fill_operation op(x);
	REQUIRE_THROWS_MATCHES(
		op.sanitize_operands(
			make_span(output_descriptors), 
			{}
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"fill_operation requires output descriptor to be initialized."
		)
	);
}
