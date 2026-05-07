// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/r2c_fft_operation_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;
using ShapePair = std::pair<shape_type, shape_type>;

TEST_CASE(
    "r2c_fft_operation_shape_policy::get_transformed_axis returns the "
    "axis provided at construction",
    "[r2c_fft_operation_shape_policy]"
)
{
    CHECK( r2c_fft_operation_shape_policy(0).get_transformed_axis() == 0 );
    CHECK( r2c_fft_operation_shape_policy(3).get_transformed_axis() == 3 );
}

TEST_CASE(
    "r2c_fft_operation_shape_policy::deduce compresses the transformed "
    "axis from N to N/2+1",
    "[r2c_fft_operation_shape_policy]"
)
{
    const auto [axis, tc] = GENERATE(table<std::size_t, ShapePair>({
        { 0, { {8, 5},    {5, 5}    } },
        { 0, { {9, 5},    {5, 5}    } },
        { 1, { {4, 8},    {4, 5}    } },
        { 1, { {4, 9},    {4, 5}    } },
        { 2, { {3, 4, 8}, {3, 4, 5} } },
        { 2, { {3, 4, 9}, {3, 4, 5} } },
    }));

    const auto& [input_shape, expected_output] = tc;

    r2c_fft_operation_shape_policy pol(axis);

    const std::vector<shape_type> inputs  = { input_shape };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == expected_output );
}

TEST_CASE(
    "r2c_fft_operation_shape_policy::deduce leaves non-transformed axes "
    "unchanged",
    "[r2c_fft_operation_shape_policy]"
)
{
    r2c_fft_operation_shape_policy pol(1);

    const std::vector<shape_type> inputs  = { {3, 10, 7} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0][0] == 3 );
    CHECK( outputs[0][2] == 7 );
}

TEST_CASE(
    "r2c_fft_operation_shape_policy::deduce throws when transformed_axis "
    "is out of range",
    "[r2c_fft_operation_shape_policy]"
)
{
    r2c_fft_operation_shape_policy pol(3);

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
