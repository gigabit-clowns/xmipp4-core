// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/data_type_policies/complex_from_real_data_type_policy.hpp>

#include <xmipp4/core/span.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("complex_from_real_data_type_policy get should return a singleton", "[complex_from_real_data_type_policy]")
{
	const auto &a = complex_from_real_data_type_policy::get();
	const auto &b = complex_from_real_data_type_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("complex_from_real_data_type_policy infer_output should produce complex type for matching real inputs", "[complex_from_real_data_type_policy]")
{
	const auto &policy = complex_from_real_data_type_policy::get();

	auto [input_type, expected_output] = GENERATE(table<numerical_type, numerical_type>({
		{ numerical_type::float16, numerical_type::complex_float16 },
		{ numerical_type::float32, numerical_type::complex_float32 },
		{ numerical_type::float64, numerical_type::complex_float64 }
	}));

	std::vector<numerical_type> inputs = { input_type, input_type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == expected_output );
}

TEST_CASE("complex_from_real_data_type_policy infer_output should reject mismatched real inputs", "[complex_from_real_data_type_policy]")
{
	const auto &policy = complex_from_real_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::float32, numerical_type::float64
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("complex_from_real_data_type_policy infer_output should reject non-real inputs", "[complex_from_real_data_type_policy]")
{
	const auto &policy = complex_from_real_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int8,
		numerical_type::int32,
		numerical_type::uint64,
		numerical_type::boolean,
		numerical_type::complex_float32
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("complex_from_real_data_type_policy validate should succeed with matching types", "[complex_from_real_data_type_policy]")
{
	const auto &policy = complex_from_real_data_type_policy::get();

	auto [input_type, output_type] = GENERATE(table<numerical_type, numerical_type>({
		{ numerical_type::float16, numerical_type::complex_float16 },
		{ numerical_type::float32, numerical_type::complex_float32 },
		{ numerical_type::float64, numerical_type::complex_float64 }
	}));

	std::vector<numerical_type> inputs = { input_type, input_type };
	std::vector<numerical_type> outputs = { output_type };

	REQUIRE_NOTHROW( policy.validate(make_span(outputs), make_span(inputs)) );
}

TEST_CASE("complex_from_real_data_type_policy validate should reject non-real inputs", "[complex_from_real_data_type_policy]")
{
	const auto &policy = complex_from_real_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int8,
		numerical_type::int32,
		numerical_type::boolean
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs = { numerical_type::complex_float32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("complex_from_real_data_type_policy validate should reject output not matching make_complex of input", "[complex_from_real_data_type_policy]")
{
	const auto &policy = complex_from_real_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::float32, numerical_type::float32
	};
	std::vector<numerical_type> outputs = { numerical_type::complex_float64 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("complex_from_real_data_type_policy validate should reject mismatched inputs", "[complex_from_real_data_type_policy]")
{
	const auto &policy = complex_from_real_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::float32, numerical_type::float64
	};
	std::vector<numerical_type> outputs = { numerical_type::complex_float32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}
