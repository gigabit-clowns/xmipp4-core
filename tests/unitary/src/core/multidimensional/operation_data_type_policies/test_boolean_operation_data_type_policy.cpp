// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/boolean_operation_data_type_policy.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
    "boolean_operation_data_type_policy::get returns a singleton",
    "[boolean_operation_data_type_policy]"
)
{
    CHECK( &boolean_operation_data_type_policy::get() ==
           &boolean_operation_data_type_policy::get() );
}

TEST_CASE(
    "boolean_operation_data_type_policy::deduce fills all outputs with "
    "boolean when all inputs are boolean",
    "[boolean_operation_data_type_policy]"
)
{
    const auto& pol = boolean_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::boolean
    };
    std::vector<numerical_type> outputs(2, numerical_type::unknown);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == numerical_type::boolean );
    CHECK( outputs[1] == numerical_type::boolean );
}

TEST_CASE(
    "boolean_operation_data_type_policy::deduce throws when any input is "
    "not boolean",
    "[boolean_operation_data_type_policy]"
)
{
    const auto& pol = boolean_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::int32
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "boolean_operation_data_type_policy::deduce throws for non-boolean input",
    "[boolean_operation_data_type_policy]"
)
{
    const auto& pol = boolean_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs  = { numerical_type::float32 };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
