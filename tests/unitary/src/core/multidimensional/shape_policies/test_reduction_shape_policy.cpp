// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/shape_policies/reduction_shape_policy.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("reduction_shape_policy infer_output should reduce specified axes", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 1 };
	reduction_shape_policy policy(make_span(reduction_axes));

	const std::vector<std::size_t> extents = { 3, 4, 5 };
	auto input = strided_layout::make_contiguous_layout(make_span(extents));

	std::vector<strided_layout> inputs = { input };
	std::vector<strided_layout> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	// Output should have extents {3, 4, 5} but axis 1 broadcasted (stride=0)
	std::vector<std::size_t> out_extents;
	outputs[0].get_extents(out_extents);
	CHECK( out_extents == extents );

	std::vector<std::ptrdiff_t> out_strides;
	outputs[0].get_strides(out_strides);
	CHECK( out_strides[1] == 0 ); // Reduced axis has zero stride
}

TEST_CASE("reduction_shape_policy infer_output should reduce multiple axes", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 0, 2 };
	reduction_shape_policy policy(make_span(reduction_axes));

	const std::vector<std::size_t> extents = { 3, 4, 5 };
	auto input = strided_layout::make_contiguous_layout(make_span(extents));

	std::vector<strided_layout> inputs = { input };
	std::vector<strided_layout> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	std::vector<std::ptrdiff_t> out_strides;
	outputs[0].get_strides(out_strides);
	CHECK( out_strides[0] == 0 ); // Reduced axis
	CHECK( out_strides[1] != 0 ); // Non-reduced axis
	CHECK( out_strides[2] == 0 ); // Reduced axis
}

TEST_CASE("reduction_shape_policy infer_output should broadcast compatible inputs", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 0 };
	reduction_shape_policy policy(make_span(reduction_axes));

	const std::vector<std::size_t> extents1 = { 3, 1 };
	const std::vector<std::size_t> extents2 = { 1, 4 };
	auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));
	auto input2 = strided_layout::make_contiguous_layout(make_span(extents2));

	std::vector<strided_layout> inputs = { input1, input2 };
	std::vector<strided_layout> outputs(1);

	policy.infer_output(make_span(outputs), make_span(inputs));

	// Inputs should be broadcasted to {3, 4}
	std::vector<std::size_t> in1_extents, in2_extents;
	inputs[0].get_extents(in1_extents);
	inputs[1].get_extents(in2_extents);

	const std::vector<std::size_t> expected = { 3, 4 };
	CHECK( in1_extents == expected );
	CHECK( in2_extents == expected );
}

TEST_CASE("reduction_shape_policy infer_output should throw when inputs are empty", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 0 };
	reduction_shape_policy policy(make_span(reduction_axes));

	std::vector<strided_layout> inputs;
	std::vector<strided_layout> outputs(1);

	REQUIRE_THROWS_AS(
		policy.infer_output(make_span(outputs), make_span(inputs)),
		std::invalid_argument
	);
}

TEST_CASE("reduction_shape_policy infer_output should fill multiple outputs", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 1 };
	reduction_shape_policy policy(make_span(reduction_axes));

	const std::vector<std::size_t> extents = { 2, 3 };
	auto input = strided_layout::make_contiguous_layout(make_span(extents));

	std::vector<strided_layout> inputs = { input };
	std::vector<strided_layout> outputs(2);

	policy.infer_output(make_span(outputs), make_span(inputs));

	for (std::size_t i = 0; i < outputs.size(); ++i)
	{
		std::vector<std::ptrdiff_t> strides;
		outputs[i].get_strides(strides);
		CHECK( strides[1] == 0 ); // Reduced axis
	}
}

TEST_CASE("reduction_shape_policy validate should succeed with correct reduction layout", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 1 };
	reduction_shape_policy policy(make_span(reduction_axes));

	// Build an output with axis 1 reduced (extent 1, broadcasted to full shape)
	const std::vector<std::size_t> reduced_extents = { 3, 1, 5 };
	const std::vector<std::size_t> full_extents = { 3, 4, 5 };
	auto output = strided_layout::make_contiguous_layout(make_span(reduced_extents));
	output = output.broadcast_to(make_span(full_extents));

	auto input = strided_layout::make_contiguous_layout(make_span(full_extents));

	std::vector<strided_layout> outputs = { output };
	std::vector<strided_layout> inputs = { input };

	REQUIRE_NOTHROW(
		policy.validate(
			span<const strided_layout>(outputs.data(), outputs.size()),
			make_span(inputs)
		)
	);
}

TEST_CASE("reduction_shape_policy validate should throw when reduction axis has non-zero stride", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 1 };
	reduction_shape_policy policy(make_span(reduction_axes));

	// Contiguous output - axis 1 will have non-zero stride
	const std::vector<std::size_t> extents = { 3, 4, 5 };
	auto output = strided_layout::make_contiguous_layout(make_span(extents));
	auto input = strided_layout::make_contiguous_layout(make_span(extents));

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

TEST_CASE("reduction_shape_policy validate should throw when non-reduction axis has zero stride", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 1 };
	reduction_shape_policy policy(make_span(reduction_axes));

	// Create output where axis 0 is broadcasted (zero stride) but is not a reduction axis
	const std::vector<std::size_t> base_extents = { 1, 1, 5 };
	const std::vector<std::size_t> full_extents = { 3, 4, 5 };
	auto output = strided_layout::make_contiguous_layout(make_span(base_extents));
	output = output.broadcast_to(make_span(full_extents));

	auto input = strided_layout::make_contiguous_layout(make_span(full_extents));

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

TEST_CASE("reduction_shape_policy validate should throw when inputs are empty", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 0 };
	reduction_shape_policy policy(make_span(reduction_axes));

	std::vector<strided_layout> inputs;

	const std::vector<std::size_t> extents = { 3, 4 };
	auto output = strided_layout::make_contiguous_layout(make_span(extents));
	std::vector<strided_layout> outputs = { output };

	REQUIRE_THROWS_AS(
		policy.validate(
			span<const strided_layout>(outputs.data(), outputs.size()),
			make_span(inputs)
		),
		std::invalid_argument
	);
}

TEST_CASE("reduction_shape_policy validate should throw when multiple outputs have different extents", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 0 };
	reduction_shape_policy policy(make_span(reduction_axes));

	const std::vector<std::size_t> extents1 = { 3, 4 };
	const std::vector<std::size_t> extents2 = { 3, 5 };
	auto output1 = strided_layout::make_contiguous_layout(make_span(extents1));
	auto output2 = strided_layout::make_contiguous_layout(make_span(extents2));

	auto input = strided_layout::make_contiguous_layout(make_span(extents1));

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

TEST_CASE("reduction_shape_policy validate should broadcast inputs to output shape", "[reduction_shape_policy]")
{
	const std::vector<std::size_t> reduction_axes = { 0 };
	reduction_shape_policy policy(make_span(reduction_axes));

	const std::vector<std::size_t> out_base_extents = { 1, 4 };
	const std::vector<std::size_t> full_extents = { 3, 4 };
	auto output = strided_layout::make_contiguous_layout(make_span(out_base_extents));
	output = output.broadcast_to(make_span(full_extents));

	const std::vector<std::size_t> in_extents = { 1, 4 };
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
	CHECK( broadcasted_extents == full_extents );
}
