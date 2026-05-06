// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operation_data_type_policies/homogeneous_arithmetic_operation_data_type_policy.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE(
    "homogeneous_arithmetic_operation_data_type_policy::get returns a singleton",
    "[homogeneous_arithmetic_operation_data_type_policy]"
)
{
    CHECK( &homogeneous_arithmetic_operation_data_type_policy::get() ==
           &homogeneous_arithmetic_operation_data_type_policy::get() );
}

TEST_CASE(
    "homogeneous_arithmetic_operation_data_type_policy::deduce accepts "
    "signed integer types",
    "[homogeneous_arithmetic_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_arithmetic_operation_data_type_policy::get();
    auto t = GENERATE(
        numerical_type::int8,
        numerical_type::int32,
        numerical_type::int64
    );

    const std::vector<numerical_type> inputs  = { t };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };
    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == t );
}

TEST_CASE(
    "homogeneous_arithmetic_operation_data_type_policy::deduce accepts "
    "unsigned integer types",
    "[homogeneous_arithmetic_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_arithmetic_operation_data_type_policy::get();
    auto t = GENERATE(
        numerical_type::uint8,
        numerical_type::uint32,
        numerical_type::uint64
    );

    const std::vector<numerical_type> inputs  = { t };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };
    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == t );
}

TEST_CASE(
    "homogeneous_arithmetic_operation_data_type_policy::deduce accepts "
    "floating-point types",
    "[homogeneous_arithmetic_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_arithmetic_operation_data_type_policy::get();
    auto t = GENERATE(
        numerical_type::float16,
        numerical_type::float32,
        numerical_type::float64
    );

    const std::vector<numerical_type> inputs  = { t };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };
    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == t );
}

TEST_CASE(
    "homogeneous_arithmetic_operation_data_type_policy::deduce accepts "
    "complex types",
    "[homogeneous_arithmetic_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_arithmetic_operation_data_type_policy::get();
    auto t = GENERATE(
        numerical_type::complex_float16,
        numerical_type::complex_float32,
        numerical_type::complex_float64
    );

    const std::vector<numerical_type> inputs  = { t };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };
    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == t );
}

TEST_CASE(
    "homogeneous_arithmetic_operation_data_type_policy::deduce throws for "
    "boolean type",
    "[homogeneous_arithmetic_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_arithmetic_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs  = { numerical_type::boolean };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "homogeneous_arithmetic_operation_data_type_policy::deduce throws when "
    "inputs differ",
    "[homogeneous_arithmetic_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_arithmetic_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::float32, numerical_type::float64
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
