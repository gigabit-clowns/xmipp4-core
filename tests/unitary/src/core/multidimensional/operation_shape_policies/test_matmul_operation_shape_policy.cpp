// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/operation_shape_policies/matmul_operation_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;

TEST_CASE(
    "matmul_operation_shape_policy::get returns a singleton",
    "[matmul_operation_shape_policy]"
)
{
    CHECK( &matmul_operation_shape_policy::get() ==
           &matmul_operation_shape_policy::get() );
}

TEST_CASE(
    "matmul_operation_shape_policy::deduce produces (n, m) output for "
    "two compatible 2D inputs",
    "[matmul_operation_shape_policy]"
)
{
    const auto& pol = matmul_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3, 4}, {4, 5} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == shape_type{3, 5} );
}

TEST_CASE(
    "matmul_operation_shape_policy::deduce produces a batched (b, n, m) "
    "output for 3D inputs",
    "[matmul_operation_shape_policy]"
)
{
    const auto& pol = matmul_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {2, 3, 4}, {2, 4, 5} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == shape_type{2, 3, 5} );
}

TEST_CASE(
    "matmul_operation_shape_policy::deduce throws when the inner dimensions "
    "do not match",
    "[matmul_operation_shape_policy]"
)
{
    const auto& pol = matmul_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3, 4}, {5, 6} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "matmul_operation_shape_policy::deduce throws when first input is 1D",
    "[matmul_operation_shape_policy]"
)
{
    const auto& pol = matmul_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {4}, {4, 5} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "matmul_operation_shape_policy::deduce throws when second input is 1D",
    "[matmul_operation_shape_policy]"
)
{
    const auto& pol = matmul_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3, 4}, {4} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
