// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <core/multidimensional/operation_shape_policies/matvec_operation_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;

TEST_CASE(
    "matvec_operation_shape_policy::get returns a singleton",
    "[matvec_operation_shape_policy]"
)
{
    CHECK( &matvec_operation_shape_policy::get() ==
           &matvec_operation_shape_policy::get() );
}

TEST_CASE(
    "matvec_operation_shape_policy::deduce produces a 1D output {n} "
    "for a 2D matrix and a compatible 1D vector",
    "[matvec_operation_shape_policy]"
)
{
    const auto& pol = matvec_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3, 4}, {4} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));

    CHECK( outputs[0] == shape_type{3} );
}

TEST_CASE(
    "matvec_operation_shape_policy::deduce throws when the matrix is not 2D",
    "[matvec_operation_shape_policy]"
)
{
    const auto& pol = matvec_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {2, 3, 4}, {4} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "matvec_operation_shape_policy::deduce throws when the vector is not 1D",
    "[matvec_operation_shape_policy]"
)
{
    const auto& pol = matvec_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3, 4}, {4, 1} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "matvec_operation_shape_policy::deduce throws when the vector length "
    "does not match the matrix column count",
    "[matvec_operation_shape_policy]"
)
{
    const auto& pol = matvec_operation_shape_policy::get();

    const std::vector<shape_type> inputs = { {3, 4}, {5} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
