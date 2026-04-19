// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/operation_shape_policies/matmul_operation_shape_policy.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("matmul_operation_shape_policy infer_output should produce correct shape for 2D inputs", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 3, 4 }; // (m=3, k=4)
    const std::vector<std::size_t> extents1 = { 4, 5 }; // (k=4, n=5)
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    policy.infer_output(make_span(outputs), make_span(inputs));

    std::vector<std::size_t> out_extents;
    outputs[0].get_extents(out_extents);

    const std::vector<std::size_t> expected = { 3, 5 };
    CHECK( out_extents == expected );
}

TEST_CASE("matmul_operation_shape_policy infer_output should produce correct shape for batched inputs", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 2, 3, 4 }; // (batch=2, m=3, k=4)
    const std::vector<std::size_t> extents1 = { 2, 4, 5 }; // (batch=2, k=4, n=5)
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    policy.infer_output(make_span(outputs), make_span(inputs));

    std::vector<std::size_t> out_extents;
    outputs[0].get_extents(out_extents);

    const std::vector<std::size_t> expected = { 2, 3, 5 };
    CHECK( out_extents == expected );
}

TEST_CASE("matmul_operation_shape_policy infer_output should broadcast batch dimensions", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 1, 3, 4 }; // batch can broadcast
    const std::vector<std::size_t> extents1 = { 2, 4, 5 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    policy.infer_output(make_span(outputs), make_span(inputs));

    std::vector<std::size_t> out_extents;
    outputs[0].get_extents(out_extents);

    const std::vector<std::size_t> expected = { 2, 3, 5 };
    CHECK( out_extents == expected );
}

TEST_CASE("matmul_operation_shape_policy infer_output should fill multiple outputs", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 3, 4 };
    const std::vector<std::size_t> extents1 = { 4, 5 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs = { input0, input1 };
    std::vector<strided_layout> outputs(3);

    policy.infer_output(make_span(outputs), make_span(inputs));

    const std::vector<std::size_t> expected = { 3, 5 };
    for (auto &out : outputs)
    {
        std::vector<std::size_t> out_extents;
        out.get_extents(out_extents);
        CHECK( out_extents == expected );
    }
}

TEST_CASE("matmul_operation_shape_policy infer_output should throw when input count is wrong", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> extents = { 3, 4 };
    auto input = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> inputs = { input }; // only 1 input
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("matmul_operation_shape_policy infer_output should throw when input[0] rank is less than 2", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 4 };     // rank 1 - invalid
    const std::vector<std::size_t> extents1 = { 4, 5 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("matmul_operation_shape_policy infer_output should throw when input[1] rank is less than 2", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 3, 4 };
    const std::vector<std::size_t> extents1 = { 4 };     // rank 1 - invalid
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("matmul_operation_shape_policy infer_output should throw when inner dimensions mismatch", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> extents0 = { 3, 4 };
    const std::vector<std::size_t> extents1 = { 7, 5 }; // k=7 != k=4
    auto input0 = strided_layout::make_contiguous_layout(make_span(extents0));
    auto input1 = strided_layout::make_contiguous_layout(make_span(extents1));

    std::vector<strided_layout> inputs = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("matmul_operation_shape_policy validate should succeed with consistent shapes", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents  = { 3, 5 };
    const std::vector<std::size_t> extents0     = { 3, 4 };
    const std::vector<std::size_t> extents1     = { 4, 5 };
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

TEST_CASE("matmul_operation_shape_policy validate should succeed with batched shapes", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents  = { 2, 3, 5 };
    const std::vector<std::size_t> extents0     = { 2, 3, 4 };
    const std::vector<std::size_t> extents1     = { 2, 4, 5 };
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

TEST_CASE("matmul_operation_shape_policy validate should broadcast batch inputs to match output", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents  = { 2, 3, 5 };
    const std::vector<std::size_t> extents0     = { 1, 3, 4 }; // broadcastable
    const std::vector<std::size_t> extents1     = { 2, 4, 5 };
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

	const std::vector<std::size_t> expected_extents = { 2, 3, 4 };
    std::vector<std::size_t> broadcasted_extents;
    inputs[0].get_extents(broadcasted_extents);
    CHECK( broadcasted_extents == expected_extents );
}

TEST_CASE("matmul_operation_shape_policy validate should throw when m dimension mismatches", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 3, 5 };
    const std::vector<std::size_t> extents0    = { 7, 4 }; // m=7 != m=3
    const std::vector<std::size_t> extents1    = { 4, 5 };
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

TEST_CASE("matmul_operation_shape_policy validate should throw when inner dimension mismatches", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 3, 5 };
    const std::vector<std::size_t> extents0    = { 3, 4 };
    const std::vector<std::size_t> extents1    = { 7, 5 }; // k=7 != k=4
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

TEST_CASE("matmul_operation_shape_policy validate should throw when n dimension mismatches", "[matmul_operation_shape_policy]")
{
    const matmul_operation_shape_policy &policy = matmul_operation_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 3, 5 };
    const std::vector<std::size_t> extents0    = { 3, 4 };
    const std::vector<std::size_t> extents1    = { 4, 9 }; // n=9 != n=5
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

TEST_CASE("matmul_operation_shape_policy get should return the same singleton instance", "[matmul_operation_shape_policy]")
{
    CHECK( &matmul_operation_shape_policy::get() == &matmul_operation_shape_policy::get() );
}
