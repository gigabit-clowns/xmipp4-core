// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/multidimensional/operation_shape_policies/reduction_shape_policy.hpp>
#include <xmipp4/core/span.hpp>

#include <stdexcept>
#include <vector>

using namespace xmipp4;
using namespace xmipp4::multidimensional;

using shape_type = std::vector<std::size_t>;

TEST_CASE(
    "reduction_operation_shape_policy constructor sorts and deduplicates axes",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({2, 0, 1, 2});

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0].empty() );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce with reduce-all and "
    "keep_dim=false produces a scalar output",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({});

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0].empty() );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce with reduce-all and "
    "keep_dim=true produces an all-ones shape",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({}, true);

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == shape_type{1, 1, 1} );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce with a single axis and "
    "keep_dim=false drops that axis",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({1});

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == shape_type{4, 6} );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce with a single axis and "
    "keep_dim=true sets that axis to 1",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({1}, true);

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == shape_type{4, 1, 6} );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce with multiple axes and "
    "keep_dim=false drops those axes",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({0, 2});

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == shape_type{5} );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce with multiple axes and "
    "keep_dim=true sets those axes to 1",
    "[reduction_operation_shape_policy]"
)
{
    // axes={0,2} on {4,5,6} → {1,5,1}
    reduction_operation_shape_policy pol({0, 2}, true);

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == shape_type{1, 5, 1} );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce broadcasts inputs before "
    "computing the reduction space",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({2});

    const std::vector<shape_type> inputs  = { {1, 5, 6}, {4, 1, 6} };
    std::vector<shape_type>       outputs(1);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == shape_type{4, 5} );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce fills all outputs "
    "homogeneously",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({1});

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(2);

    pol.deduce(make_span(outputs), make_span(inputs));
    CHECK( outputs[0] == shape_type{4, 6} );
    CHECK( outputs[1] == shape_type{4, 6} );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce throws for empty inputs",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({});

    const std::vector<shape_type> inputs;
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce throws when an axis is out "
    "of range",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({3}); // rank of input is 3

    const std::vector<shape_type> inputs  = { {4, 5, 6} };
    std::vector<shape_type>       outputs(1);

    CHECK_THROWS_AS(
        pol.deduce(make_span(outputs), make_span(inputs)),
        std::invalid_argument
    );
}

TEST_CASE(
    "reduction_operation_shape_policy::deduce validates axes against the "
    "broadcasted rank, not individual input ranks",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({1});

    const std::vector<shape_type> inputs  = { {5}, {4, 5} };
    std::vector<shape_type>       outputs(1);

    CHECK_NOTHROW(pol.deduce(make_span(outputs), make_span(inputs)));
    CHECK( outputs[0] == shape_type{4} );
}

TEST_CASE(
    "reduction_operation_shape_policy::get_reduction_axes returns the "
    "sorted deduplicated axes provided at construction",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({2, 0, 1, 2});
    const auto axes = pol.get_reduction_axes();

    REQUIRE( axes.size() == 3 );
    CHECK( axes[0] == 0 );
    CHECK( axes[1] == 1 );
    CHECK( axes[2] == 2 );
}

TEST_CASE(
    "reduction_operation_shape_policy::get_reduction_axes returns an empty "
    "span when constructed with no axes",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({});
    CHECK( pol.get_reduction_axes().empty() );
}

TEST_CASE(
    "reduction_operation_shape_policy::keeps_dimensions returns false "
    "by default",
    "[reduction_operation_shape_policy]"
)
{
    reduction_operation_shape_policy pol({0});
    CHECK( pol.keeps_dimensions() == false );
}

TEST_CASE(
    "reduction_operation_shape_policy::keeps_dimensions returns the value "
    "provided at construction",
    "[reduction_operation_shape_policy]"
)
{
    CHECK( reduction_operation_shape_policy({0}, false).keeps_dimensions() == false );
    CHECK( reduction_operation_shape_policy({0}, true ).keeps_dimensions() == true  );
}
