// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <core/multidimensional/data_type_policies/copy_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("copy_data_type_policy get should return a singleton", "[copy_data_type_policy]")
{
	const auto &a = copy_data_type_policy::get();
	const auto &b = copy_data_type_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("copy_data_type_policy infer_output should fill output when input is valid", "[copy_data_type_policy]")
{
	const auto &policy = copy_data_type_policy::get();

	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint32,
		numerical_type::float32,
		numerical_type::float64
	);
	std::vector<numerical_type> inputs = { type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
}

TEST_CASE("copy_data_type_policy infer_output with invalid output count should fail", "[copy_data_type_policy]")
{
	const auto &policy = copy_data_type_policy::get();

	const auto output_count = GENERATE(0, 2, 8);
	std::vector<numerical_type> inputs = { numerical_type::uint32 };
	std::vector<numerical_type> outputs(output_count);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("copy_data_type_policy infer_output with invalid input count should fail", "[copy_data_type_policy]")
{
	const auto &policy = copy_data_type_policy::get();

	const auto input_count = GENERATE(0, 2, 8);
	std::vector<numerical_type> inputs(input_count, numerical_type::uint32);
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("copy_data_type_policy validate with correct input should succeed", "[copy_data_type_policy]")
{
	const auto &policy = copy_data_type_policy::get();

	const auto type1 = GENERATE(
		numerical_type::uint8,
		numerical_type::int8,
		numerical_type::float16,
		numerical_type::float64,
		numerical_type::complex_float32
	);
	const auto type2 = GENERATE(
		numerical_type::complex_float16,
		numerical_type::float32,
		numerical_type::float16,
		numerical_type::int64,
		numerical_type::uint32
	);

	std::vector<numerical_type> inputs = { type1 };
	std::vector<numerical_type> outputs = { type2 };

	REQUIRE_NOTHROW(
		policy.validate(make_span(outputs), make_span(inputs))
	);
}

TEST_CASE("copy_data_type_policy validate with invalid output count should fail", "[copy_data_type_policy]")
{
	const auto &policy = copy_data_type_policy::get();

	const auto output_count = GENERATE(0, 2, 8);
	std::vector<numerical_type> inputs = { numerical_type::uint32 };
	std::vector<numerical_type> outputs(output_count);

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("copy_data_type_policy validate with invalid input count should fail", "[copy_data_type_policy]")
{
	const auto &policy = copy_data_type_policy::get();

	const auto input_count = GENERATE(0, 2, 8);
	std::vector<numerical_type> inputs(input_count, numerical_type::uint32);
	std::vector<numerical_type> outputs = { numerical_type::uint32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}
