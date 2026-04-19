// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/homogeneous_operation_shape_policy.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("homogeneous_operation_shape_policy get should return a singleton", "[homogeneous_operation_shape_policy]")
{
    const auto &a = homogeneous_operation_shape_policy::get();
    const auto &b = homogeneous_operation_shape_policy::get();
    CHECK( &a == &b );
}

TEST_CASE("homogeneous_operation_shape_policy infer_output should assign input shape to single output", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    const std::vector<std::size_t> extents = { 3, 4, 5 };
    auto input = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> inputs = { input };
    std::vector<strided_layout> outputs(1);

    policy.infer_output(make_span(outputs), make_span(inputs));

    std::vector<std::size_t> out_extents;
    outputs[0].get_extents(out_extents);
    CHECK( out_extents == extents );
}

TEST_CASE("homogeneous_operation_shape_policy infer_output should assign input shape to multiple outputs", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

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

TEST_CASE("homogeneous_operation_shape_policy infer_output should succeed when all inputs have the same shape", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    const std::vector<std::size_t> extents = { 2, 5 };
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents));
    auto input2 = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> inputs = { input1, input2 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_NOTHROW( policy.infer_output(make_span(outputs), make_span(inputs)) );

    std::vector<std::size_t> out_extents;
    outputs[0].get_extents(out_extents);
    CHECK( out_extents == extents );
}

TEST_CASE("homogeneous_operation_shape_policy infer_output should throw when inputs are empty", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    std::vector<strided_layout> inputs;
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("homogeneous_operation_shape_policy infer_output should throw when inputs have different shapes", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    const std::vector<std::size_t> extents1 = { 3, 4 };
    const std::vector<std::size_t> extents2 = { 3, 5 };
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));
    auto input2 = strided_layout::make_contiguous_layout(make_span(extents2));

    std::vector<strided_layout> inputs = { input1, input2 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("homogeneous_operation_shape_policy validate should succeed when all operands have the same shape", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    const std::vector<std::size_t> extents = { 4, 6 };
    auto output = strided_layout::make_contiguous_layout(make_span(extents));
    auto input  = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> outputs = { output };
    std::vector<strided_layout> inputs  = { input };

    REQUIRE_NOTHROW(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        )
    );
}

TEST_CASE("homogeneous_operation_shape_policy validate should succeed with multiple identical outputs and inputs", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    const std::vector<std::size_t> extents = { 2, 3 };
    auto layout = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> outputs = { layout, layout };
    std::vector<strided_layout> inputs  = { layout, layout };

    REQUIRE_NOTHROW(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        )
    );
}

TEST_CASE("homogeneous_operation_shape_policy validate should throw when outputs are empty", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    const std::vector<std::size_t> extents = { 3, 4 };
    auto input = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> outputs;
    std::vector<strided_layout> inputs = { input };

    REQUIRE_THROWS_AS(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        ),
        std::invalid_argument
    );
}

TEST_CASE("homogeneous_operation_shape_policy validate should throw when outputs have different shapes", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    const std::vector<std::size_t> extents1 = { 3, 4 };
    const std::vector<std::size_t> extents2 = { 3, 5 };
    auto output1 = strided_layout::make_contiguous_layout(make_span(extents1));
    auto output2 = strided_layout::make_contiguous_layout(make_span(extents2));
    auto input   = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> outputs = { output1, output2 };
    std::vector<strided_layout> inputs  = { input };

    REQUIRE_THROWS_AS(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        ),
        std::invalid_argument
    );
}

TEST_CASE("homogeneous_operation_shape_policy validate should throw when an input has a different shape from the output", "[homogeneous_operation_shape_policy]")
{
    const auto &policy = homogeneous_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 3, 4 };
    const std::vector<std::size_t> in_extents  = { 3, 5 };
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
    auto input  = strided_layout::make_contiguous_layout(make_span(in_extents));

    std::vector<strided_layout> outputs = { output };
    std::vector<strided_layout> inputs  = { input };

    REQUIRE_THROWS_AS(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        ),
        std::invalid_argument
    );
}
