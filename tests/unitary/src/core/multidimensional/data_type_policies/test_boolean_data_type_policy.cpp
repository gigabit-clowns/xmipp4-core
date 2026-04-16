// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/data_type_policies/boolean_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("boolean_data_type_policy get should return a singleton", "[boolean_data_type_policy]")
{
	const auto &a = boolean_data_type_policy::get();
	const auto &b = boolean_data_type_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("boolean_data_type_policy infer_output should fill outputs with boolean when inputs are boolean", "[boolean_data_type_policy]")
{
	const auto &policy = boolean_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::boolean, numerical_type::boolean
	};
	std::vector<numerical_type> outputs(2);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == numerical_type::boolean );
	CHECK( outputs[1] == numerical_type::boolean );
}

TEST_CASE("boolean_data_type_policy infer_output should work with a single input", "[boolean_data_type_policy]")
{
	const auto &policy = boolean_data_type_policy::get();

	std::vector<numerical_type> inputs = { numerical_type::boolean };
	std::vector<numerical_type> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	CHECK( outputs[0] == numerical_type::boolean );
}

TEST_CASE("boolean_data_type_policy infer_output should throw when inputs are not boolean", "[boolean_data_type_policy]")
{
	const auto &policy = boolean_data_type_policy::get();

	auto type = GENERATE(
		numerical_type::int8,
		numerical_type::uint32,
		numerical_type::float32,
		numerical_type::float64,
		numerical_type::complex_float32
	);

	std::vector<numerical_type> inputs = { type };
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("boolean_data_type_policy infer_output should throw when some inputs are not boolean", "[boolean_data_type_policy]")
{
	const auto &policy = boolean_data_type_policy::get();

	std::vector<numerical_type> inputs = {
		numerical_type::boolean, numerical_type::int32
	};
	std::vector<numerical_type> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("boolean_data_type_policy validate should succeed when all types are boolean", "[boolean_data_type_policy]")
{
	const auto &policy = boolean_data_type_policy::get();

	std::vector<numerical_type> outputs = {
		numerical_type::boolean, numerical_type::boolean
	};
	std::vector<numerical_type> inputs = {
		numerical_type::boolean, numerical_type::boolean
	};

	REQUIRE_NOTHROW(
		policy.validate(make_span(outputs), make_span(inputs))
	);
}

TEST_CASE("boolean_data_type_policy validate should throw when outputs are not boolean", "[boolean_data_type_policy]")
{
	const auto &policy = boolean_data_type_policy::get();

	std::vector<numerical_type> outputs = { numerical_type::float32 };
	std::vector<numerical_type> inputs = { numerical_type::boolean };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("boolean_data_type_policy validate should throw when inputs are not boolean", "[boolean_data_type_policy]")
{
	const auto &policy = boolean_data_type_policy::get();

	std::vector<numerical_type> outputs = { numerical_type::boolean };
	std::vector<numerical_type> inputs = { numerical_type::int32 };

	REQUIRE_THROWS_AS(
		policy.validate(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}
