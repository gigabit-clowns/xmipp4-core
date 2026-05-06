// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/operation_shape_policies/dot_operation_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;

TEST_CASE(
    "dot_operation_shape_policy::get returns a singleton",
    "[dot_operation_shape_policy]"
)
{
    CHECK( &dot_operation_shape_policy::get() ==
           &dot_operation_shape_policy::get() );
}

TEST_CASE(
    "dot_operation_shape_policy::deduce produces a scalar output for "
    "two 1D inputs of equal length",
    "[dot_operation_shape_policy]"
)
{
    const auto& pol = dot_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {8}, {8} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0].empty() );
}

TEST_CASE(
    "dot_operation_shape_policy::deduce throws when first input is not 1D",
    "[dot_operation_shape_policy]"
)
{
    const auto& pol = dot_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {4, 8}, {8} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "dot_operation_shape_policy::deduce throws when second input is not 1D",
    "[dot_operation_shape_policy]"
)
{
    const auto& pol = dot_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {8}, {4, 8} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "dot_operation_shape_policy::deduce throws when input lengths differ",
    "[dot_operation_shape_policy]"
)
{
    const auto& pol = dot_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {8}, {9} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
