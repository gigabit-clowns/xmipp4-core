// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/complex_from_real_operation_data_type_policy.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
    "complex_from_real_operation_data_type_policy::get returns a singleton",
    "[complex_from_real_operation_data_type_policy]"
)
{
    CHECK( &complex_from_real_operation_data_type_policy::get() ==
           &complex_from_real_operation_data_type_policy::get() );
}

TEST_CASE(
    "complex_from_real_operation_data_type_policy::deduce produces the "
    "complex counterpart for floating-point inputs",
    "[complex_from_real_operation_data_type_policy]"
)
{
    const auto& pol = complex_from_real_operation_data_type_policy::get();

    const std::vector<std::pair<numerical_type, numerical_type>> cases = {
        { numerical_type::float16, numerical_type::complex_float16 },
        { numerical_type::float32, numerical_type::complex_float32 },
        { numerical_type::float64, numerical_type::complex_float64 },
    };

    for (const auto& [input, expected] : cases)
    {
        const std::vector<numerical_type> inputs  = { input };
        std::vector<numerical_type>       outputs = { numerical_type::unknown };
        pol.deduce(make_span(outputs), make_span(inputs));
        CHECK( outputs[0] == expected );
    }
}

TEST_CASE(
    "complex_from_real_operation_data_type_policy::deduce fills all outputs "
    "with the complex counterpart",
    "[complex_from_real_operation_data_type_policy]"
)
{
    const auto& pol = complex_from_real_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs  = { numerical_type::float32,
                                                   numerical_type::float32 };
    std::vector<numerical_type>       outputs(2, numerical_type::unknown);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == numerical_type::complex_float32 );
    CHECK( outputs[1] == numerical_type::complex_float32 );
}

TEST_CASE(
    "complex_from_real_operation_data_type_policy::deduce throws for "
    "integer input",
    "[complex_from_real_operation_data_type_policy]"
)
{
    const auto& pol = complex_from_real_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs  = { numerical_type::int32 };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "complex_from_real_operation_data_type_policy::deduce throws when "
    "inputs differ",
    "[complex_from_real_operation_data_type_policy]"
)
{
    const auto& pol = complex_from_real_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::float32, numerical_type::float64
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
