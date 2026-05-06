// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/elementwise_operation_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;

TEST_CASE(
    "elementwise_operation_shape_policy::get returns a singleton",
    "[elementwise_operation_shape_policy]"
)
{
    CHECK( &elementwise_operation_shape_policy::get() ==
           &elementwise_operation_shape_policy::get() );
}

TEST_CASE(
    "elementwise_operation_shape_policy::deduce fills all outputs with "
    "the broadcast shape of all inputs",
    "[elementwise_operation_shape_policy]"
)
{
    const auto& pol = elementwise_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {1, 8}, {4, 1} };
    std::vector<shape_type> outputs(2);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == shape_type{4, 8} );
    CHECK( outputs[1] == shape_type{4, 8} );
}

TEST_CASE(
    "elementwise_operation_shape_policy::deduce works with a single input",
    "[elementwise_operation_shape_policy]"
)
{
    const auto& pol = elementwise_operation_shape_policy::get();

    const std::vector<shape_type> inputs  = { {3, 5} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == shape_type{3, 5} );
}

TEST_CASE(
    "elementwise_operation_shape_policy::deduce throws for empty inputs",
    "[elementwise_operation_shape_policy]"
)
{
    const auto& pol = elementwise_operation_shape_policy::get();

    const std::vector<shape_type> inputs;
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "elementwise_operation_shape_policy::accept does not throw when all user "
    "outputs can be broadcasted to the canonical shape",
    "[elementwise_operation_shape_policy]"
)
{
    const auto& pol = elementwise_operation_shape_policy::get();

    const std::vector<shape_type> canonical    = { {1, 8}, {1, 8} };
    const std::vector<shape_type> inputs       = { {1, 8} };
    const std::vector<shape_type> user_outputs = { {4, 8}, {4, 8} };

    CHECK_NOTHROW(
        pol.accept(
			make_span(user_outputs), 
			make_span(canonical),
            make_span(inputs)
		)
    );
}

TEST_CASE(
    "elementwise_operation_shape_policy::accept throws when user outputs "
    "have different shapes",
    "[elementwise_operation_shape_policy]"
)
{
    const auto& pol = elementwise_operation_shape_policy::get();

    const std::vector<shape_type> canonical    = { {4, 8} };
    const std::vector<shape_type> inputs       = { {4, 8} };
    const std::vector<shape_type> user_outputs = { {4, 9} };

    CHECK_THROWS_AS(
        pol.accept(make_span(user_outputs), make_span(canonical),
                   make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "elementwise_operation_shape_policy::accept throws when canonical shape "
    "is not broadcast-compatible with the user output",
    "[elementwise_operation_shape_policy]"
)
{
    const auto& pol = elementwise_operation_shape_policy::get();

    const std::vector<shape_type> canonical    = { {4, 8} };
    const std::vector<shape_type> inputs       = { {4, 8} };
    const std::vector<shape_type> user_outputs = { {3, 8} };

    CHECK_THROWS_AS(
        pol.accept(make_span(user_outputs), make_span(canonical),
                   make_span(inputs)),
        std::invalid_argument
    );
}
