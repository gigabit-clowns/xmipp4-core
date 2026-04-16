// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/data_type_policies/same_arithmetic_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("same_arithmetic_data_type_policy get should return a singleton", "[same_arithmetic_data_type_policy]")
{
	const auto &a = same_arithmetic_data_type_policy::get();
	const auto &b = same_arithmetic_data_type_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("same_arithmetic_data_type_policy infer_output should accept signed integers", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int8,
		numerical_type::int16,
		numerical_type::int32,
		numerical_type::int64
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
}

TEST_CASE("same_arithmetic_data_type_policy infer_output should accept unsigned integers", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::uint8,
		numerical_type::uint16,
		numerical_type::uint32,
		numerical_type::uint64
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
}

TEST_CASE("same_arithmetic_data_type_policy infer_output should accept floating point types", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
}

TEST_CASE("same_arithmetic_data_type_policy infer_output should accept complex types", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::complex_float16,
		numerical_type::complex_float32,
		numerical_type::complex_float64
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
}

TEST_CASE("same_arithmetic_data_type_policy infer_output should reject boolean type", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::boolean, numerical_type::boolean
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("same_arithmetic_data_type_policy infer_output should reject character type", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::char8, numerical_type::char8
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("same_arithmetic_data_type_policy infer_output should throw when inputs differ", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::int32, numerical_type::float32
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("same_arithmetic_data_type_policy validate should succeed with matching arithmetic types", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int32,
		numerical_type::uint16,
		numerical_type::float64,
		numerical_type::complex_float32
	);

	std::vector<numerical_type> outputs = { type };
	std::vector<numerical_type> inputs = { type, type };

	REQUIRE_NOTHROW(
		policy.validate(make_span(outputs), make_span(inputs))
	);
}

TEST_CASE("same_arithmetic_data_type_policy validate should throw for boolean outputs", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	std::vector<numerical_type> outputs = { numerical_type::boolean };
	std::vector<numerical_type> inputs = { numerical_type::boolean };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("same_arithmetic_data_type_policy validate should throw when inputs do not match outputs", "[same_arithmetic_data_type_policy]")
{
	const auto &policy = same_arithmetic_data_type_policy::get();

	std::vector<numerical_type> outputs = { numerical_type::float32 };
	std::vector<numerical_type> inputs = { numerical_type::float64 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}
