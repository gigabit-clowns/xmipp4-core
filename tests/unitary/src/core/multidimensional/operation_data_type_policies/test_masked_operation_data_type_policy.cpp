// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/masked_operation_data_type_policy.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
    "masked_operation_data_type_policy::get returns a singleton",
    "[masked_operation_data_type_policy]"
)
{
    CHECK( &masked_operation_data_type_policy::get() ==
           &masked_operation_data_type_policy::get() );
}

TEST_CASE(
    "masked_operation_data_type_policy::deduce propagates the value type "
    "when mask is boolean and both values share the same valid type",
    "[masked_operation_data_type_policy]"
)
{
    const auto& pol = masked_operation_data_type_policy::get();
    auto t = GENERATE(
        numerical_type::int32,
        numerical_type::float32,
        numerical_type::complex_float32
    );

    const std::vector<numerical_type> inputs  = {
        numerical_type::boolean, t, t
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };
    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == t );
}

TEST_CASE(
    "masked_operation_data_type_policy::deduce throws when mask is not boolean",
    "[masked_operation_data_type_policy]"
)
{
    const auto& pol = masked_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::int32, numerical_type::float32, numerical_type::float32
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "masked_operation_data_type_policy::deduce throws when value types differ",
    "[masked_operation_data_type_policy]"
)
{
    const auto& pol = masked_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::float32, numerical_type::float64
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "masked_operation_data_type_policy::deduce throws when value type is unknown",
    "[masked_operation_data_type_policy]"
)
{
    const auto& pol = masked_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::boolean,
        numerical_type::unknown,
        numerical_type::unknown
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
