// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <core/multidimensional/operation_data_type_policies/select_operation_data_type_policy.hpp>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

TEST_CASE("select_operation_data_type_policy get should return a singleton", "[select_operation_data_type_policy]")
{
    const auto &a = select_operation_data_type_policy::get();
    const auto &b = select_operation_data_type_policy::get();
    CHECK( &a == &b );
}

TEST_CASE("select_operation_data_type_policy infer_output should set output to the value type", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    auto type = GENERATE(
        numerical_type::int8,
        numerical_type::uint32,
        numerical_type::float32,
        numerical_type::float64,
        numerical_type::complex_float32
    );

    std::vector<numerical_type> inputs = { numerical_type::boolean, type, type };
    std::vector<numerical_type> outputs(1);

    policy.infer_output(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == type );
}

TEST_CASE("select_operation_data_type_policy infer_output should throw when mask is not boolean", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    auto mask_type = GENERATE(
        numerical_type::int8,
        numerical_type::uint8,
        numerical_type::float32
    );

    std::vector<numerical_type> inputs = {
        mask_type, numerical_type::float32, numerical_type::float32
    };
    std::vector<numerical_type> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy infer_output should throw when x and y types differ", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::float32, numerical_type::float64
    };
    std::vector<numerical_type> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy infer_output should throw when value type is unknown", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::unknown, numerical_type::unknown
    };
    std::vector<numerical_type> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy infer_output should throw when input count is not 3", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    auto input_count = GENERATE(0, 1, 2, 4, 10);

    std::vector<numerical_type> inputs(input_count, numerical_type::float32);
    std::vector<numerical_type> outputs(1);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy infer_output should throw when output count is not 1", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    auto output_count = GENERATE(0, 2, 3, 10);

    std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::float32, numerical_type::float32
    };
    std::vector<numerical_type> outputs(output_count);

    REQUIRE_THROWS_AS(
        policy.infer_output(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy validate should succeed with valid operands", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    auto type = GENERATE(
        numerical_type::int8,
        numerical_type::uint32,
        numerical_type::float32,
        numerical_type::float64,
        numerical_type::complex_float32
    );

    std::vector<numerical_type> inputs = { numerical_type::boolean, type, type };
    std::vector<numerical_type> outputs = { type };

    REQUIRE_NOTHROW(
        policy.validate(make_span(outputs), make_span(inputs))
    );
}

TEST_CASE("select_operation_data_type_policy validate should throw when mask is not boolean", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    auto mask_type = GENERATE(
        numerical_type::int8,
        numerical_type::uint8,
        numerical_type::float32
    );

    std::vector<numerical_type> inputs = {
        mask_type, numerical_type::float32, numerical_type::float32
    };
    std::vector<numerical_type> outputs = { numerical_type::float32 };

    REQUIRE_THROWS_AS(
        policy.validate(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy validate should throw when x does not match output", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::float64, numerical_type::float32
    };
    std::vector<numerical_type> outputs = { numerical_type::float32 };

    REQUIRE_THROWS_AS(
        policy.validate(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy validate should throw when y does not match output", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::float32, numerical_type::float64
    };
    std::vector<numerical_type> outputs = { numerical_type::float32 };

    REQUIRE_THROWS_AS(
        policy.validate(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy validate should throw when output type is unknown", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::float32, numerical_type::float32
    };
    std::vector<numerical_type> outputs = { numerical_type::unknown };

    REQUIRE_THROWS_AS(
        policy.validate(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy validate should throw when input count is not 3", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    auto input_count = GENERATE(0, 1, 2, 4, 10);

    std::vector<numerical_type> inputs(input_count, numerical_type::float32);
    std::vector<numerical_type> outputs = { numerical_type::float32 };

    REQUIRE_THROWS_AS(
        policy.validate(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE("select_operation_data_type_policy validate should throw when output count is not 1", "[select_operation_data_type_policy]")
{
    const auto &policy = select_operation_data_type_policy::get();

    auto output_count = GENERATE(0, 2, 3, 10);

    std::vector<numerical_type> inputs = {
        numerical_type::boolean, numerical_type::float32, numerical_type::float32
    };
    std::vector<numerical_type> outputs(output_count, numerical_type::float32);

    REQUIRE_THROWS_AS(
        policy.validate(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
