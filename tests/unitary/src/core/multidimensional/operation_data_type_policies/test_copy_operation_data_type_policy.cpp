// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <core/multidimensional/operation_data_type_policies/copy_operation_data_type_policy.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
    "copy_operation_data_type_policy::get returns a singleton",
    "[copy_operation_data_type_policy]"
)
{
    CHECK( &copy_operation_data_type_policy::get() ==
           &copy_operation_data_type_policy::get() );
}

TEST_CASE(
    "copy_operation_data_type_policy::deduce propagates the input type "
    "to the output",
    "[copy_operation_data_type_policy]"
)
{
    const auto& pol = copy_operation_data_type_policy::get();
    auto t = GENERATE(
        numerical_type::boolean,
        numerical_type::int32,
        numerical_type::float64,
        numerical_type::complex_float32
    );

    const std::vector<numerical_type> inputs  = { t };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };
    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == t );
}

TEST_CASE(
    "copy_operation_data_type_policy::deduce throws for unknown input type",
    "[copy_operation_data_type_policy]"
)
{
    const auto& pol = copy_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs  = { numerical_type::unknown };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "copy_operation_data_type_policy::accept allows any valid user output "
    "type regardless of the canonical type",
    "[copy_operation_data_type_policy]"
)
{
    const auto& pol = copy_operation_data_type_policy::get();
    auto t = GENERATE(
        numerical_type::boolean,
        numerical_type::int32,
        numerical_type::float32,
        numerical_type::float64,
        numerical_type::complex_float32
    );

    const std::vector<numerical_type> user_out = { t };
    const std::vector<numerical_type> canonical = { numerical_type::float32 };
    const std::vector<numerical_type> inputs    = { numerical_type::float32 };

    CHECK_NOTHROW(
        pol.accept(make_span(user_out), make_span(canonical), make_span(inputs))
    );
}

TEST_CASE(
    "copy_operation_data_type_policy::accept throws for unknown user output",
    "[copy_operation_data_type_policy]"
)
{
    const auto& pol = copy_operation_data_type_policy::get();

    const std::vector<numerical_type> user_out = { numerical_type::unknown };
    const std::vector<numerical_type> canonical = { numerical_type::float32 };
    const std::vector<numerical_type> inputs    = { numerical_type::float32 };

    CHECK_THROWS_AS(
        pol.accept(make_span(user_out), make_span(canonical), make_span(inputs)),
        std::invalid_argument
    );
}
