// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/operation_data_type_policies/abs_operation_data_type_policy.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
    "abs_operation_data_type_policy::get returns a singleton",
    "[abs_operation_data_type_policy]"
)
{
    CHECK( &abs_operation_data_type_policy::get() ==
           &abs_operation_data_type_policy::get() );
}

TEST_CASE(
    "abs_operation_data_type_policy::deduce returns the same type for "
    "non-complex arithmetic inputs",
    "[abs_operation_data_type_policy]"
)
{
    const auto& pol = abs_operation_data_type_policy::get();

    for (auto t : { numerical_type::int32, numerical_type::uint32,
                    numerical_type::float32, numerical_type::float64 })
    {
        const std::vector<numerical_type> inputs  = { t };
        std::vector<numerical_type>       outputs = { numerical_type::unknown };
        pol.deduce(make_span(outputs), make_span(inputs));
        CHECK( outputs[0] == t );
    }
}

TEST_CASE(
    "abs_operation_data_type_policy::deduce returns the real counterpart "
    "for complex inputs",
    "[abs_operation_data_type_policy]"
)
{
    const auto& pol = abs_operation_data_type_policy::get();

    const std::vector<std::pair<numerical_type, numerical_type>> cases = {
        { numerical_type::complex_float16, numerical_type::float16 },
        { numerical_type::complex_float32, numerical_type::float32 },
        { numerical_type::complex_float64, numerical_type::float64 },
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
    "abs_operation_data_type_policy::deduce throws for boolean input",
    "[abs_operation_data_type_policy]"
)
{
    const auto& pol = abs_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs  = { numerical_type::boolean };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "abs_operation_data_type_policy::deduce throws when inputs differ",
    "[abs_operation_data_type_policy]"
)
{
    const auto& pol = abs_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::float32, numerical_type::float64
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
