// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <core/multidimensional/operation_shape_policies/c2r_fft_operation_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;
using ShapePair = std::pair<shape_type, shape_type>;

TEST_CASE(
    "c2r_fft_operation_shape_policy::get_transformed_axis returns the "
    "axis provided at construction",
    "[c2r_fft_operation_shape_policy]"
)
{
    CHECK( c2r_fft_operation_shape_policy(0, false).get_transformed_axis() == 0 );
    CHECK( c2r_fft_operation_shape_policy(3, false).get_transformed_axis() == 3 );
}

TEST_CASE(
    "c2r_fft_operation_shape_policy::is_output_odd returns the value "
    "provided at construction",
    "[c2r_fft_operation_shape_policy]"
)
{
    CHECK( c2r_fft_operation_shape_policy(0, false).is_output_odd() == false );
    CHECK( c2r_fft_operation_shape_policy(0, true ).is_output_odd() == true  );
}

TEST_CASE(
    "c2r_fft_operation_shape_policy::deduce expands the transformed axis "
    "from M to 2*(M-1) when is_output_odd=false",
    "[c2r_fft_operation_shape_policy]"
)
{
    // M=5 → N=8, M=1 → N=0 (edge), M=2 → N=2
    const auto [axis, tc] = GENERATE(table<std::size_t, ShapePair>({
        { 0, { {5, 3},    {8, 3}    } },
        { 1, { {4, 5},    {4, 8}    } },
        { 2, { {3, 4, 5}, {3, 4, 8} } },
    }));

    const auto& [input_shape, expected_output] = tc;

    c2r_fft_operation_shape_policy pol(axis, false);

    const std::vector<shape_type> inputs  = { input_shape };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == expected_output );
}

TEST_CASE(
    "c2r_fft_operation_shape_policy::deduce expands the transformed axis "
    "from M to 2*M-1 when is_output_odd=true",
    "[c2r_fft_operation_shape_policy]"
)
{
    // M=5 → N=9, M=3 → N=5
    const auto [axis, tc] = GENERATE(table<std::size_t, ShapePair>({
        { 0, { {5, 3},    {9, 3}    } },
        { 1, { {4, 5},    {4, 9}    } },
        { 2, { {3, 4, 5}, {3, 4, 9} } },
    }));

    const auto& [input_shape, expected_output] = tc;

    c2r_fft_operation_shape_policy pol(axis, true);

    const std::vector<shape_type> inputs  = { input_shape };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == expected_output );
}

TEST_CASE(
    "c2r_fft_operation_shape_policy::deduce leaves non-transformed axes "
    "unchanged",
    "[c2r_fft_operation_shape_policy]"
)
{
    c2r_fft_operation_shape_policy pol(1, false);

    const std::vector<shape_type> inputs  = { {3, 5, 7} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0][0] == 3 );
    CHECK( outputs[0][2] == 7 );
}

TEST_CASE(
    "c2r_fft_operation_shape_policy::deduce throws when transformed_axis "
    "is out of range",
    "[c2r_fft_operation_shape_policy]"
)
{
    c2r_fft_operation_shape_policy pol(3, false);

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}
