// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <core/multidimensional/operation_data_type_policies/abs_operation_data_type_policy.hpp>

#include <xmipp4/core/span.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("abs_operation_data_type_policy get should return a singleton", "[abs_operation_data_type_policy]")
{
	const auto &a = abs_operation_data_type_policy::get();
	const auto &b = abs_operation_data_type_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("abs_operation_data_type_policy infer_output should return the same type for signed integer inputs", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	const auto type = GENERATE(
		numerical_type::int8,
		numerical_type::int16,
		numerical_type::int32,
		numerical_type::int64
	);

	std::vector<numerical_type> inputs = { type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
}

TEST_CASE("abs_operation_data_type_policy infer_output should return the same type for unsigned integer inputs", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	const auto type = GENERATE(
		numerical_type::uint8,
		numerical_type::uint16,
		numerical_type::uint32,
		numerical_type::uint64
	);

	std::vector<numerical_type> inputs = { type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
}

TEST_CASE("abs_operation_data_type_policy infer_output should return the same type for floating-point inputs", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	const auto type = GENERATE(
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64
	);

	std::vector<numerical_type> inputs = { type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == type );
}

TEST_CASE("abs_operation_data_type_policy infer_output should return the real equivalent for complex inputs", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	auto [input_type, expected_output] = GENERATE(table<numerical_type, numerical_type>({
		{ numerical_type::complex_float16, numerical_type::float16 },
		{ numerical_type::complex_float32, numerical_type::float32 },
		{ numerical_type::complex_float64, numerical_type::float64 },
	}));

	std::vector<numerical_type> inputs = { input_type };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == expected_output );
}

TEST_CASE("abs_operation_data_type_policy infer_output should reject boolean input", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = { numerical_type::boolean };
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("abs_operation_data_type_policy infer_output should reject character input", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = { numerical_type::char8 };
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("abs_operation_data_type_policy validate should succeed for non-complex arithmetic types", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	const auto type = GENERATE(
		numerical_type::int32,
		numerical_type::uint16,
		numerical_type::float64
	);

	std::vector<numerical_type> inputs = { type };
	std::vector<numerical_type> outputs = { type };

	REQUIRE_NOTHROW( policy.validate(make_span(outputs), make_span(inputs)) );
}

TEST_CASE("abs_operation_data_type_policy validate should succeed for complex inputs with real outputs", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	auto [input_type, output_type] = GENERATE(table<numerical_type, numerical_type>({
		{ numerical_type::complex_float16, numerical_type::float16 },
		{ numerical_type::complex_float32, numerical_type::float32 },
		{ numerical_type::complex_float64, numerical_type::float64 },
	}));

	std::vector<numerical_type> inputs = { input_type };
	std::vector<numerical_type> outputs = { output_type };

	REQUIRE_NOTHROW( policy.validate(make_span(outputs), make_span(inputs)) );
}

TEST_CASE("abs_operation_data_type_policy validate should reject complex output for complex input", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = { numerical_type::complex_float32 };
	std::vector<numerical_type> outputs = { numerical_type::complex_float32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("abs_operation_data_type_policy validate should reject mismatched output for non-complex input", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = { numerical_type::float32 };
	std::vector<numerical_type> outputs = { numerical_type::float64 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("abs_operation_data_type_policy validate should reject boolean input", "[abs_operation_data_type_policy]")
{
	const auto &policy = abs_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = { numerical_type::boolean };
	std::vector<numerical_type> outputs = { numerical_type::boolean };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}
