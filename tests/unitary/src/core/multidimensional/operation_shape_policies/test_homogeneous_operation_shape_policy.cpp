// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/homogeneous_operation_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;

TEST_CASE(
    "homogeneous_operation_shape_policy::get returns a singleton",
    "[homogeneous_operation_shape_policy]"
)
{
    CHECK( &homogeneous_operation_shape_policy::get() ==
           &homogeneous_operation_shape_policy::get() );
}

TEST_CASE(
    "homogeneous_operation_shape_policy::deduce fills all outputs with "
    "the common input shape",
    "[homogeneous_operation_shape_policy]"
)
{
    const auto& pol = homogeneous_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {4, 8}, {4, 8} };
    std::vector<shape_type> outputs(2);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == shape_type{4, 8} );
    CHECK( outputs[1] == shape_type{4, 8} );
}

TEST_CASE(
    "homogeneous_operation_shape_policy::deduce works with a single input",
    "[homogeneous_operation_shape_policy]"
)
{
    const auto& pol = homogeneous_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3, 5} };
    std::vector<shape_type> outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == shape_type{3, 5} );
}

TEST_CASE(
    "homogeneous_operation_shape_policy::deduce throws when input shapes differ",
    "[homogeneous_operation_shape_policy]"
)
{
    const auto& pol = homogeneous_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {4, 8}, {4, 9} };
    std::vector<shape_type> outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "homogeneous_operation_shape_policy::deduce throws for empty inputs",
    "[homogeneous_operation_shape_policy]"
)
{
    const auto& pol = homogeneous_operation_shape_policy::get();

    const std::vector<shape_type> inputs;
    std::vector<shape_type> outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
