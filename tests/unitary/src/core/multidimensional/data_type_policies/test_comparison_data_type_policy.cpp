// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <core/multidimensional/data_type_policies/comparison_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("comparison_data_type_policy get should return a singleton", "[comparison_data_type_policy]")
{
	const auto &a = comparison_data_type_policy::get();
	const auto &b = comparison_data_type_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("comparison_data_type_policy infer_output should fill output when input is valid", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint32,
		numerical_type::float32,
		numerical_type::float64
	);
	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == numerical_type::boolean );
}

TEST_CASE("comparison_data_type_policy infer_output should throw when inputs are invalid types", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64,
		numerical_type::unknown
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("comparison_data_type_policy infer_output should throw when input count is invalid", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint32,
		numerical_type::float32,
		numerical_type::float64
	);

	auto input_count = GENERATE(0, 1, 3, 10);

	std::vector<numerical_type> inputs(input_count, type);
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("comparison_data_type_policy infer_output should throw when output count is invalid", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();
	auto output_count = GENERATE(0, 2, 3, 10);

	std::vector<numerical_type> inputs = {
		numerical_type::float32, numerical_type::float32
	};
	std::vector<numerical_type> outputs(output_count);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("comparison_data_type_policy infer_output should throw when provided with different input types", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();

	auto type1 = GENERATE(
		numerical_type::int8,
		numerical_type::uint32,
		numerical_type::float32,
		numerical_type::float64
	);

	auto type2 = GENERATE(
		numerical_type::uint8,
		numerical_type::int32,
		numerical_type::float16
	);

	std::vector<numerical_type> inputs = {type1, type2};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("comparison_data_type_policy validate should succeed with valid operands", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();

	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint32,
		numerical_type::float32,
		numerical_type::float64
	);


	std::vector<numerical_type> inputs = {type, type};
	std::vector<numerical_type> outputs = { numerical_type::boolean };

	policy.validate(make_span(outputs), make_span(inputs));
}


TEST_CASE("comparison_data_type_policy validate should throw when inputs are invalid types", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64,
		numerical_type::unknown
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs = { numerical_type::boolean };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("comparison_data_type_policy validate should throw when input count is invalid", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint32,
		numerical_type::float32,
		numerical_type::float64
	);

	auto input_count = GENERATE(0, 1, 3, 10);

	std::vector<numerical_type> inputs(input_count, type);
	std::vector<numerical_type> outputs = { numerical_type::boolean };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("comparison_data_type_policy validate should throw when output count is invalid", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();
	auto output_count = GENERATE(0, 2, 3, 10);

	std::vector<numerical_type> inputs = {
		numerical_type::float32, numerical_type::float32
	};
	std::vector<numerical_type> outputs(output_count, numerical_type::boolean);

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("comparison_data_type_policy validate should throw when output is not boolean", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();
	auto output_count = GENERATE(0, 2, 3, 10);

	std::vector<numerical_type> inputs = {
		numerical_type::float32, numerical_type::float32
	};
	std::vector<numerical_type> outputs = { numerical_type::char8 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("comparison_data_type_policy validate should throw when provided with different input types", "[comparison_data_type_policy]")
{
	const auto &policy = comparison_data_type_policy::get();

	auto type1 = GENERATE(
		numerical_type::int8,
		numerical_type::uint32,
		numerical_type::float32,
		numerical_type::float64
	);

	auto type2 = GENERATE(
		numerical_type::uint8,
		numerical_type::int32,
		numerical_type::float16
	);

	std::vector<numerical_type> inputs = {type1, type2};
	std::vector<numerical_type> outputs = { numerical_type::boolean };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}
