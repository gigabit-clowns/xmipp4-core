// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/operation_shape_policies/outer_operation_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;

TEST_CASE(
    "outer_operation_shape_policy::get returns a singleton",
    "[outer_operation_shape_policy]"
)
{
    CHECK( &outer_operation_shape_policy::get() ==
           &outer_operation_shape_policy::get() );
}

TEST_CASE(
    "outer_operation_shape_policy::deduce produces a {n, m} 2D output "
    "for two 1D inputs",
    "[outer_operation_shape_policy]"
)
{
    const auto& pol = outer_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3}, {5} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == shape_type{3, 5} );
}

TEST_CASE(
    "outer_operation_shape_policy::deduce throws when first input is not 1D",
    "[outer_operation_shape_policy]"
)
{
    const auto& pol = outer_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3, 4}, {5} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "outer_operation_shape_policy::deduce throws when second input is not 1D",
    "[outer_operation_shape_policy]"
)
{
    const auto& pol = outer_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3}, {5, 2} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
