// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/shape_policies/elementwise_shape_policy.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("elementwise_shape_policy get should return a singleton", "[elementwise_shape_policy]")
{
	const auto &a = elementwise_shape_policy::get();
	const auto &b = elementwise_shape_policy::get();
	CHECK( &a == &b );
}

TEST_CASE("elementwise_shape_policy infer_output should produce contiguous output from single input", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	const std::vector<std::size_t> extents = { 3, 4, 5 };
	auto input = strided_layout::make_contiguous_layout(make_span(extents));

	std::vector<strided_layout> inputs = { input };
	std::vector<strided_layout> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	const auto expected = strided_layout::make_contiguous_layout(make_span(extents));
	std::vector<std::size_t> out_extents;
	outputs[0].get_extents(out_extents);
	CHECK( out_extents == extents );
}

TEST_CASE("elementwise_shape_policy infer_output should broadcast compatible inputs", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	const std::vector<std::size_t> extents1 = { 3, 1, 5 };
	const std::vector<std::size_t> extents2 = { 1, 4, 5 };
	auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));
	auto input2 = strided_layout::make_contiguous_layout(make_span(extents2));

	std::vector<strided_layout> inputs = { input1, input2 };
	std::vector<strided_layout> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	const std::vector<std::size_t> expected_extents = { 3, 4, 5 };
	std::vector<std::size_t> out_extents;
	outputs[0].get_extents(out_extents);
	CHECK( out_extents == expected_extents );
}

TEST_CASE("elementwise_shape_policy infer_output should broadcast inputs to output shape", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	const std::vector<std::size_t> extents1 = { 1, 4 };
	const std::vector<std::size_t> extents2 = { 3, 1 };
	auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));
	auto input2 = strided_layout::make_contiguous_layout(make_span(extents2));

	std::vector<strided_layout> inputs = { input1, input2 };
	std::vector<strided_layout> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	// After infer_output, inputs should be broadcasted
	std::vector<std::size_t> input1_extents, input2_extents;
	inputs[0].get_extents(input1_extents);
	inputs[1].get_extents(input2_extents);

	const std::vector<std::size_t> expected = { 3, 4 };
	CHECK( input1_extents == expected );
	CHECK( input2_extents == expected );
}

TEST_CASE("elementwise_shape_policy infer_output should fill multiple outputs", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	const std::vector<std::size_t> extents = { 2, 3 };
	auto input = strided_layout::make_contiguous_layout(make_span(extents));

	std::vector<strided_layout> inputs = { input };
	std::vector<strided_layout> outputs(3);

	policy.infer_output(make_span(outputs), make_span(inputs));

	for (std::size_t i = 0; i < outputs.size(); ++i)
	{
		std::vector<std::size_t> out_extents;
		outputs[i].get_extents(out_extents);
		CHECK( out_extents == extents );
	}
}

TEST_CASE("elementwise_shape_policy infer_output should throw when inputs are empty", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	std::vector<strided_layout> inputs;
	std::vector<strided_layout> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("elementwise_shape_policy validate should succeed with matching shapes", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	const std::vector<std::size_t> extents = { 3, 4, 5 };
	auto output = strided_layout::make_contiguous_layout(make_span(extents));
	auto input = strided_layout::make_contiguous_layout(make_span(extents));

	std::vector<strided_layout> outputs = { output };
	std::vector<strided_layout> inputs = { input };

	REQUIRE_NOTHROW(
		policy.validate(
			span<const strided_layout>(outputs.data(), outputs.size()),
			make_span(inputs)
		)
	);
}

TEST_CASE("elementwise_shape_policy validate should broadcast inputs to output shape", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	const std::vector<std::size_t> out_extents = { 3, 4 };
	const std::vector<std::size_t> in_extents = { 1, 4 };
	auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
	auto input = strided_layout::make_contiguous_layout(make_span(in_extents));

	std::vector<strided_layout> outputs = { output };
	std::vector<strided_layout> inputs = { input };

	REQUIRE_NOTHROW(
		policy.validate(
			span<const strided_layout>(outputs.data(), outputs.size()),
			make_span(inputs)
		)
	);

	std::vector<std::size_t> broadcasted_extents;
	inputs[0].get_extents(broadcasted_extents);
	CHECK( broadcasted_extents == out_extents );
}

TEST_CASE("elementwise_shape_policy validate should throw when outputs are empty", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();


	const std::vector<std::size_t> extents = { 3, 4 };
	auto input = strided_layout::make_contiguous_layout(make_span(extents));
	std::vector<strided_layout> inputs = {input};
	std::vector<strided_layout> outputs;

	REQUIRE_THROWS_AS(
		policy.validate(
			span<const strided_layout>(outputs.data(), outputs.size()),
			make_span(inputs)
		),
		std::invalid_argument
	);
}

TEST_CASE("elementwise_shape_policy validate should throw when multiple outputs have different extents", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	const std::vector<std::size_t> extents1 = { 3, 4 };
	const std::vector<std::size_t> extents2 = { 3, 5 };
	auto output1 = strided_layout::make_contiguous_layout(make_span(extents1));
	auto output2 = strided_layout::make_contiguous_layout(make_span(extents2));

	const std::vector<std::size_t> in_extents = { 3, 1 };
	auto input = strided_layout::make_contiguous_layout(make_span(in_extents));

	std::vector<strided_layout> outputs = { output1, output2 };
	std::vector<strided_layout> inputs = { input };

	REQUIRE_THROWS_AS(
		policy.validate(
			span<const strided_layout>(outputs.data(), outputs.size()),
			make_span(inputs)
		),
		std::invalid_argument
	);
}

TEST_CASE("elementwise_shape_policy validate should throw when output has broadcasted (zero stride) dimensions", "[elementwise_shape_policy]")
{
	const auto &policy = elementwise_shape_policy::get();

	// Create a broadcasted output layout (extent > 1 with stride 0)
	const std::vector<std::size_t> base_extents = { 1, 4 };
	const std::vector<std::size_t> broadcast_extents = { 3, 4 };
	auto output = strided_layout::make_contiguous_layout(make_span(base_extents));
	output = output.broadcast_to(make_span(broadcast_extents));

	auto input = strided_layout::make_contiguous_layout(make_span(broadcast_extents));

	std::vector<strided_layout> outputs = { output };
	std::vector<strided_layout> inputs = { input };

	REQUIRE_THROWS_AS(
		policy.validate(
			span<const strided_layout>(outputs.data(), outputs.size()),
			make_span(inputs)
		),
		std::invalid_argument
	);
}
