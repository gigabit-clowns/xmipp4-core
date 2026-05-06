// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <core/multidimensional/operation_data_type_policies/comparison_operation_data_type_policy.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
    "comparison_operation_data_type_policy::get returns a singleton",
    "[comparison_operation_data_type_policy]"
)
{
    CHECK( &comparison_operation_data_type_policy::get() ==
           &comparison_operation_data_type_policy::get() );
}

TEST_CASE(
    "comparison_operation_data_type_policy::deduce produces boolean output "
    "for non-complex same-type inputs",
    "[comparison_operation_data_type_policy]"
)
{
    const auto& pol = comparison_operation_data_type_policy::get();
    auto t = GENERATE(
        numerical_type::boolean,
        numerical_type::int32,
        numerical_type::uint32,
        numerical_type::float32
    );

    const std::vector<numerical_type> inputs  = { t, t };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };
    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == numerical_type::boolean );
}

TEST_CASE(
    "comparison_operation_data_type_policy::deduce throws for complex input",
    "[comparison_operation_data_type_policy]"
)
{
    const auto& pol = comparison_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::complex_float32, numerical_type::complex_float32
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "comparison_operation_data_type_policy::deduce throws when input types differ",
    "[comparison_operation_data_type_policy]"
)
{
    const auto& pol = comparison_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::int32, numerical_type::float32
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
