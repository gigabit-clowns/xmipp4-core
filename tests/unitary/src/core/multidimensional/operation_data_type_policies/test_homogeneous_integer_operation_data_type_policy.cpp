// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_integer_operation_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("homogeneous_integer_operation_data_type_policy get should return a singleton", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &a = homogeneous_integer_operation_data_type_policy::get();
	const auto &b = homogeneous_integer_operation_data_type_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("homogeneous_integer_operation_data_type_policy infer_output should accept signed integers", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

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

TEST_CASE("homogeneous_integer_operation_data_type_policy infer_output should accept unsigned integers", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

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

TEST_CASE("homogeneous_integer_operation_data_type_policy infer_output should reject floating point types", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::float16,
		numerical_type::float32,
		numerical_type::float64
	);

	std::vector<numerical_type> inputs = { type, type };
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_integer_operation_data_type_policy infer_output should reject complex types", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::complex_float32, numerical_type::complex_float32
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_integer_operation_data_type_policy infer_output should reject boolean type", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::boolean, numerical_type::boolean
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_integer_operation_data_type_policy infer_output should throw when inputs differ", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::int32, numerical_type::int64
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_integer_operation_data_type_policy validate should succeed with matching integer types", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int16,
		numerical_type::uint32,
		numerical_type::int64
	);

	std::vector<numerical_type> outputs = { type };
	std::vector<numerical_type> inputs = { type, type };

	REQUIRE_NOTHROW(
		policy.validate(make_span(outputs), make_span(inputs))
	);
}

TEST_CASE("homogeneous_integer_operation_data_type_policy validate should throw for non-integer outputs", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

	std::vector<numerical_type> outputs = { numerical_type::float32 };
	std::vector<numerical_type> inputs = { numerical_type::float32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("homogeneous_integer_operation_data_type_policy validate should throw when inputs do not match outputs", "[homogeneous_integer_operation_data_type_policy]")
{
	const auto &policy = homogeneous_integer_operation_data_type_policy::get();

	std::vector<numerical_type> outputs = { numerical_type::int32 };
	std::vector<numerical_type> inputs = { numerical_type::int64 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}
