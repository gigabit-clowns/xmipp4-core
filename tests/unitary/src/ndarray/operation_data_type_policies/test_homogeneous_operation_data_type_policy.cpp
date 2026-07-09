// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/ndarray/operation_data_type_policies/homogeneous_operation_data_type_policy.hpp>
#include <xmipp4/core/span.hpp>
#include <xmipp4/core/numerical_type.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::dispatch;

TEST_CASE(
    "homogeneous_operation_data_type_policy::get returns a singleton",
    "[homogeneous_operation_data_type_policy]"
)
{
    CHECK( &homogeneous_operation_data_type_policy::get() ==
           &homogeneous_operation_data_type_policy::get() );
}

TEST_CASE(
    "homogeneous_operation_data_type_policy::deduce fills all outputs "
    "with the common input type",
    "[homogeneous_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::float32, numerical_type::float32
    };
    std::vector<numerical_type> outputs(2, numerical_type::unknown);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == numerical_type::float32 );
    CHECK( outputs[1] == numerical_type::float32 );
}

TEST_CASE(
    "homogeneous_operation_data_type_policy::deduce throws when inputs differ",
    "[homogeneous_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs = {
        numerical_type::float32, numerical_type::float64
    };
    std::vector<numerical_type> outputs(1, numerical_type::unknown);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "homogeneous_operation_data_type_policy::deduce throws for unknown type",
    "[homogeneous_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs  = { numerical_type::unknown };
    std::vector<numerical_type>       outputs = { numerical_type::unknown };

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "homogeneous_operation_data_type_policy::deduce yields unknown types for "
    "empty inputs",
    "[homogeneous_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs;
    std::vector<numerical_type>       outputs(2, numerical_type::float32);

    pol.deduce(make_span(outputs), make_span(inputs));

    // With no inputs the output type cannot be deduced from them, so it is
    // left unknown for accept() to resolve from the user outputs.
    CHECK( outputs[0] == numerical_type::unknown );
    CHECK( outputs[1] == numerical_type::unknown );
}

TEST_CASE(
    "homogeneous_operation_data_type_policy::accept adopts the user output "
    "type when there are no inputs",
    "[homogeneous_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs;
    const std::vector<numerical_type> canonical = {
        numerical_type::unknown, numerical_type::unknown
    };
    const std::vector<numerical_type> user_outputs = {
        numerical_type::float32, numerical_type::float32
    };

    CHECK_NOTHROW(
        pol.accept(
            make_span(user_outputs),
            make_span(canonical),
            make_span(inputs)
        )
    );
}

TEST_CASE(
    "homogeneous_operation_data_type_policy::accept throws when user outputs "
    "disagree and there are no inputs",
    "[homogeneous_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs;
    const std::vector<numerical_type> canonical = {
        numerical_type::unknown, numerical_type::unknown
    };
    const std::vector<numerical_type> user_outputs = {
        numerical_type::float32, numerical_type::float64
    };

    CHECK_THROWS_AS(
        pol.accept(
            make_span(user_outputs),
            make_span(canonical),
            make_span(inputs)
        ),
        std::invalid_argument
    );
}

TEST_CASE(
    "homogeneous_operation_data_type_policy::accept requires user outputs to "
    "match the canonical type when inputs are present",
    "[homogeneous_operation_data_type_policy]"
)
{
    const auto& pol = homogeneous_operation_data_type_policy::get();

    const std::vector<numerical_type> inputs       = { numerical_type::float32 };
    const std::vector<numerical_type> canonical    = { numerical_type::float32 };
    const std::vector<numerical_type> user_outputs = { numerical_type::float64 };

    CHECK_THROWS_AS(
        pol.accept(
            make_span(user_outputs),
            make_span(canonical),
            make_span(inputs)
        ),
        std::invalid_argument
    );
}
