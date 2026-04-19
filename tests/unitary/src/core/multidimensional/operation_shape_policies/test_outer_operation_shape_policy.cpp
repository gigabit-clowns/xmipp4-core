// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/operation_shape_policies/outer_operation_shape_policy.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("outer_operation_shape_policy infer_output should produce a 2D output of shape (m, n)", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 4 }; // m=4
    const std::vector<std::size_t> extents1 = { 7 }; // n=7
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    policy.infer_output(make_span(outputs), make_span(inputs));

    std::vector<std::size_t> out_extents;
    outputs[0].get_extents(out_extents);

    const std::vector<std::size_t> expected = { 4, 7 };
    CHECK( out_extents == expected );
}

TEST_CASE("outer_operation_shape_policy infer_output should fill multiple outputs", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 3 };
    const std::vector<std::size_t> extents1 = { 5 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(2);

    policy.infer_output(make_span(outputs), make_span(inputs));

    const std::vector<std::size_t> expected = { 3, 5 };
    for (auto &out : outputs)
    {
        std::vector<std::size_t> out_extents;
        out.get_extents(out_extents);
        CHECK( out_extents == expected );
    }
}

TEST_CASE("outer_operation_shape_policy infer_output should throw when input count is wrong", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> extents = { 4 };
    auto input = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> inputs  = { input }; // only 1
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("outer_operation_shape_policy infer_output should throw when input[0] is not 1D", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 3, 4 }; // rank 2 - invalid
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

TEST_CASE("outer_operation_shape_policy infer_output should throw when input[1] is not 1D", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 4 };
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

TEST_CASE("outer_operation_shape_policy validate should succeed with consistent shapes", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 4, 7 };
    const std::vector<std::size_t> extents0    = { 4 };
    const std::vector<std::size_t> extents1    = { 7 };
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> outputs = { output };
    std::vector<strided_layout> inputs  = { input0, input1 };

    REQUIRE_NOTHROW(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        )
    );
}

TEST_CASE("outer_operation_shape_policy validate should throw when output is not 2D", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 4 }; // rank 1 - invalid
    const std::vector<std::size_t> extents0    = { 4 };
    const std::vector<std::size_t> extents1    = { 7 };
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
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

TEST_CASE("outer_operation_shape_policy validate should throw when input[0] length does not match row count", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 4, 7 };
    const std::vector<std::size_t> extents0    = { 9 }; // 9 != 4
    const std::vector<std::size_t> extents1    = { 7 };
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
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

TEST_CASE("outer_operation_shape_policy validate should throw when input[1] length does not match column count", "[outer_operation_shape_policy]")
{
    const outer_operation_shape_policy &policy = outer_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 4, 7 };
    const std::vector<std::size_t> extents0    = { 4 };
    const std::vector<std::size_t> extents1    = { 3 }; // 3 != 7
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
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

TEST_CASE("outer_operation_shape_policy get should return the same singleton instance", "[outer_operation_shape_policy]")
{
    CHECK( &outer_operation_shape_policy::get() == &outer_operation_shape_policy::get() );
}
