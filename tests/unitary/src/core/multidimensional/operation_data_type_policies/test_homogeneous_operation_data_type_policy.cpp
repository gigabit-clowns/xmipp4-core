// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_operation_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("homogeneous_operation_data_type_policy get should return a singleton", "[homogeneous_operation_data_type_policy]")
{
	const auto &a = homogeneous_operation_data_type_policy::get();
	const auto &b = homogeneous_operation_data_type_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("homogeneous_operation_data_type_policy infer_output should fill outputs with the common input type", "[homogeneous_operation_data_type_policy]")
{
	const auto &policy = homogeneous_operation_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint16,
		numerical_type::int32,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float32
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(2);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
	CHECK( outputs[1] == type );
}

TEST_CASE("homogeneous_operation_data_type_policy infer_output should work with a single input", "[homogeneous_operation_data_type_policy]")
{
	const auto &policy = homogeneous_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = { numerical_type::float64 };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == numerical_type::float64 );
}

TEST_CASE("homogeneous_operation_data_type_policy infer_output should throw when inputs are empty", "[homogeneous_operation_data_type_policy]")
{
	const auto &policy = homogeneous_operation_data_type_policy::get();

	std::vector<numerical_type> inputs;
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_operation_data_type_policy infer_output should throw when inputs have different types", "[homogeneous_operation_data_type_policy]")
{
	const auto &policy = homogeneous_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::float32, numerical_type::float64
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_operation_data_type_policy validate should succeed when all types match", "[homogeneous_operation_data_type_policy]")
{
	const auto &policy = homogeneous_operation_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int32,
		numerical_type::float64,
		numerical_type::complex_float32
	);

	std::vector<numerical_type> outputs = { type };
	std::vector<numerical_type> inputs = { type, type };

	REQUIRE_NOTHROW(
		policy.validate(make_span(outputs), make_span(inputs))
	);
}

TEST_CASE("homogeneous_operation_data_type_policy validate should throw when outputs are empty", "[homogeneous_operation_data_type_policy]")
{
	const auto &policy = homogeneous_operation_data_type_policy::get();

	std::vector<numerical_type> outputs;
	std::vector<numerical_type> inputs = { numerical_type::int32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_operation_data_type_policy validate should throw when outputs differ", "[homogeneous_operation_data_type_policy]")
{
	const auto &policy = homogeneous_operation_data_type_policy::get();

	std::vector<numerical_type> outputs = {
		numerical_type::float32, numerical_type::float64
	};
	std::vector<numerical_type> inputs = { numerical_type::float32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_operation_data_type_policy validate should throw when inputs do not match outputs", "[homogeneous_operation_data_type_policy]")
{
	const auto &policy = homogeneous_operation_data_type_policy::get();

	std::vector<numerical_type> outputs = { numerical_type::int32 };
	std::vector<numerical_type> inputs = { numerical_type::float32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}
