// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operations/copy_operation.hpp>

#include <xmipp4/core/multidimensional/array_descriptor.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("get_name in copy_operation should return copy", "[copy_operation]")
{
	copy_operation op;
	CHECK( op.get_name() == "copy" );
}

TEST_CASE("sanitize_operands in copy_operation should copy input descriptor to the output when none is provided", "[copy_operation]")
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

	std::array<array_descriptor, 1> output_descriptors = {};
	std::array<array_descriptor, 1> input_descriptors = {descriptor};

	copy_operation op;
	op.sanitize_operands(
		make_span(output_descriptors), 
		make_span(input_descriptors)
	);

	CHECK( output_descriptors[0] == descriptor );
	CHECK( input_descriptors[0] == descriptor );
}

TEST_CASE("sanitize_operands in copy_operation should broadcast input to the output when output is provided", "[copy_operation]")
{
	std::vector<std::size_t> extents1 = {1, 2, 4};
	std::vector<std::size_t> extents2 = {20, 2, 4};
	const auto data_type1 = GENERATE(
		numerical_type::int16,
		numerical_type::float32
	);
	const auto data_type2 = GENERATE(
		numerical_type::float64,
		numerical_type::complex_float32
	);
	const array_descriptor descriptor1(
		strided_layout::make_contiguous_layout(make_span(extents1)),
		data_type1
	);
	const array_descriptor descriptor2(
		strided_layout::make_contiguous_layout(make_span(extents2)),
		data_type2
	);
	const array_descriptor broadcasted_descriptor1(
		descriptor1.get_layout().broadcast_to(make_span(extents2)),
		data_type1
	);

	std::array<array_descriptor, 1> output_descriptors = {descriptor2};
	std::array<array_descriptor, 1> input_descriptors = {descriptor1};

	copy_operation op;
	op.sanitize_operands(
		make_span(output_descriptors), 
		make_span(input_descriptors)
	);

	CHECK( output_descriptors[0] == descriptor2 );
	CHECK( input_descriptors[0] == broadcasted_descriptor1 );
}

TEST_CASE("sanitize_operands in copy_operation should throw if an invalid amount of inputs is provided", "[copy_operation]")
{
	const auto n_input = GENERATE(0, 2, 20);

	std::array<array_descriptor, 1> output_descriptors = {};
	std::vector<array_descriptor> input_descriptors(n_input);

	copy_operation op;
	REQUIRE_THROWS_MATCHES(
		op.sanitize_operands(
			make_span(output_descriptors), 
			make_span(input_descriptors)
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"copy_operation requires exactly one input operand."
		)
	);
}

TEST_CASE("sanitize_operands in copy_operation should throw if an invalid amount of outputs is provided", "[copy_operation]")
{
	const auto n_outputs = GENERATE(0, 2, 20);

	std::vector<array_descriptor> output_descriptors(n_outputs);
	std::array<array_descriptor, 1> input_descriptors = {};

	copy_operation op;
	REQUIRE_THROWS_MATCHES(
		op.sanitize_operands(
			make_span(output_descriptors), 
			make_span(input_descriptors)
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"copy_operation requires exactly one output operand."
		)
	);
}

TEST_CASE("sanitize_operands in copy_operation should throw if output is not initialized", "[copy_operation]")
{
	std::array<array_descriptor, 1> output_descriptors = {};
	std::array<array_descriptor, 1> input_descriptors = {};

	copy_operation op;
	REQUIRE_THROWS_MATCHES(
		op.sanitize_operands(
			make_span(output_descriptors), 
			make_span(input_descriptors)
		),
		std::invalid_argument,
		Catch::Matchers::Message(
			"copy_operation requires input descriptor to be initialized."
		)
	);
}
