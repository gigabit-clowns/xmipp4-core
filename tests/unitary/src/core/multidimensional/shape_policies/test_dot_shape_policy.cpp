// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/shape_policies/dot_shape_policy.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("dot_shape_policy infer_output should produce a scalar (rank-0) output", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents = { 8 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    policy.infer_output(make_span(outputs), make_span(inputs));

    std::vector<std::size_t> out_extents;
    outputs[0].get_extents(out_extents);

    CHECK( out_extents.empty() );
}

TEST_CASE("dot_shape_policy infer_output should fill multiple outputs with scalars", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents = { 5 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(3);

    policy.infer_output(make_span(outputs), make_span(inputs));

    for (auto &out : outputs)
    {
        std::vector<std::size_t> out_extents;
        out.get_extents(out_extents);
        CHECK( out_extents.empty() );
    }
}

TEST_CASE("dot_shape_policy infer_output should throw when input count is wrong", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents = { 5 };
    auto input = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> inputs  = { input }; // only 1
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("dot_shape_policy infer_output should throw when input[0] is not 1D", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 3, 5 }; // rank 2 - invalid
    const std::vector<std::size_t> extents1 = { 5 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("dot_shape_policy infer_output should throw when input[1] is not 1D", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 5 };
    const std::vector<std::size_t> extents1 = { 3, 5 }; // rank 2 - invalid
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("dot_shape_policy infer_output should throw when input lengths differ", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 5 };
    const std::vector<std::size_t> extents1 = { 7 }; // 7 != 5
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("dot_shape_policy validate should succeed with two 1D vectors of equal length", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents = { 6 };
    const std::vector<std::size_t> scalar_extents;
    auto output = strided_layout::make_contiguous_layout(make_span(scalar_extents));
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> outputs = { output };
    std::vector<strided_layout> inputs  = { input0, input1 };

    REQUIRE_NOTHROW(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        )
    );
}

TEST_CASE("dot_shape_policy validate should throw when output is not scalar", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents     = { 6 };
    const std::vector<std::size_t> out_extents = { 1 }; // rank 1 - invalid
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> outputs = { output };
    std::vector<strided_layout> inputs  = { input0, input1 };

    REQUIRE_THROWS_AS(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        ),
        std::invalid_argument
    );
}

TEST_CASE("dot_shape_policy validate should throw when inputs have different lengths", "[dot_shape_policy]")
{
    const dot_shape_policy &policy = dot_shape_policy::get();

    const std::vector<std::size_t> extents0    = { 6 };
    const std::vector<std::size_t> extents1    = { 9 }; // 9 != 6
    const std::vector<std::size_t> scalar_extents;
    auto output = strided_layout::make_contiguous_layout(make_span(scalar_extents));
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> outputs = { output };
    std::vector<strided_layout> inputs  = { input0, input1 };

    REQUIRE_THROWS_AS(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        ),
        std::invalid_argument
    );
}

TEST_CASE("dot_shape_policy get should return the same singleton instance", "[dot_shape_policy]")
{
    CHECK( &dot_shape_policy::get() == &dot_shape_policy::get() );
}
