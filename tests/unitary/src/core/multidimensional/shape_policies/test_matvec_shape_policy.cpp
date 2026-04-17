// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/shape_policies/matvec_shape_policy.hpp>
#include <xmipp4/core/multidimensional/strided_layout.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("matvec_shape_policy infer_output should produce 1D output of length m", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> mat_extents = { 4, 6 }; // (m=4, n=6)
    const std::vector<std::size_t> vec_extents = { 6 };    // (n=6,)
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    policy.infer_output(make_span(outputs), make_span(inputs));

    std::vector<std::size_t> out_extents;
    outputs[0].get_extents(out_extents);

    const std::vector<std::size_t> expected = { 4 };
    CHECK( out_extents == expected );
}

TEST_CASE("matvec_shape_policy infer_output should fill multiple outputs", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> mat_extents = { 3, 5 };
    const std::vector<std::size_t> vec_extents = { 5 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(2);

    policy.infer_output(make_span(outputs), make_span(inputs));

    const std::vector<std::size_t> expected = { 3 };
    for (auto &out : outputs)
    {
        std::vector<std::size_t> out_extents;
        out.get_extents(out_extents);
        CHECK( out_extents == expected );
    }
}

TEST_CASE("matvec_shape_policy infer_output should throw when input count is wrong", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> extents = { 3, 4 };
    auto input = strided_layout::make_contiguous_layout(make_span(extents));

    std::vector<strided_layout> inputs  = { input };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("matvec_shape_policy infer_output should throw when matrix is not 2D", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> mat_extents = { 2, 3, 4 }; // rank 3 - invalid
    const std::vector<std::size_t> vec_extents = { 4 };
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("matvec_shape_policy infer_output should throw when vector is not 1D", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> mat_extents = { 3, 4 };
    const std::vector<std::size_t> vec_extents = { 2, 4 }; // rank 2 - invalid
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("matvec_shape_policy infer_output should throw when column count does not match vector length", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> mat_extents = { 3, 4 };
    const std::vector<std::size_t> vec_extents = { 7 }; // 7 != 4
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

    std::vector<strided_layout> inputs  = { input0, input1 };
    std::vector<strided_layout> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("matvec_shape_policy validate should succeed with consistent shapes", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 4 };
    const std::vector<std::size_t> mat_extents = { 4, 6 };
    const std::vector<std::size_t> vec_extents = { 6 };
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

    std::vector<strided_layout> outputs = { output };
    std::vector<strided_layout> inputs  = { input0, input1 };

    REQUIRE_NOTHROW(
        policy.validate(
            span<const strided_layout>(outputs.data(), outputs.size()),
            make_span(inputs)
        )
    );
}

TEST_CASE("matvec_shape_policy validate should throw when output is not 1D", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 4, 1 }; // 2D - invalid
    const std::vector<std::size_t> mat_extents = { 4, 6 };
    const std::vector<std::size_t> vec_extents = { 6 };
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

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

TEST_CASE("matvec_shape_policy validate should throw when row count does not match output", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 9 }; // m=9 - wrong
    const std::vector<std::size_t> mat_extents = { 4, 6 };
    const std::vector<std::size_t> vec_extents = { 6 };
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

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

TEST_CASE("matvec_shape_policy validate should throw when column count does not match vector", "[matvec_shape_policy]")
{
    const matvec_shape_policy &policy = matvec_shape_policy::get();

    const std::vector<std::size_t> out_extents = { 4 };
    const std::vector<std::size_t> mat_extents = { 4, 6 };
    const std::vector<std::size_t> vec_extents = { 9 }; // 9 != 6
    auto output = strided_layout::make_contiguous_layout(make_span(out_extents));
    auto input0 = strided_layout::make_contiguous_layout(make_span(mat_extents));
    auto input1 = strided_layout::make_contiguous_layout(make_span(vec_extents));

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

TEST_CASE("matvec_shape_policy get should return the same singleton instance", "[matvec_shape_policy]")
{
    CHECK( &matvec_shape_policy::get() == &matvec_shape_policy::get() );
}
