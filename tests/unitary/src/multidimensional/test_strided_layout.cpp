// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>

#include <algorithm>
#include <array>

using namespace xmipp4::multidimensional;

template <std::size_t N>
static 
strided_layout make_custom_layout(
    const std::array<std::size_t, N> &extents,
    const std::array<std::ptrdiff_t, N> &strides,
    std::ptrdiff_t offset
)
{
    return strided_layout::make_custom_layout(
        xmipp4::make_span(extents),
        xmipp4::make_span(strides),
        offset
    );
}

static 
strided_layout make_test_layout()
{
    const std::array<std::size_t, 6> extents = 
    {
        120, 56, 24, 1, 10, 8
    };
    const std::array<std::ptrdiff_t, 6> strides = 
    {
        // TODO
    };
    const auto offset = 20;

    return make_custom_layout(extents, strides, offset);
}


// TODO constructors and getters

TEST_CASE("compute_storage_requirement in strided_layout should return the correct size", "[strided_layout]")
{

}

TEST_CASE("compute_storage_requirement in strided_layout should return 0 if there is a zero-sized axis", "[strided_layout]")
{

}

TEST_CASE( "apply_subscripts in strided_layout should implicitly fill the reminding axes", "[strided_layout]" )
{

}

TEST_CASE( "apply_subscripts in strided_layout should implicitly fill the reminding axes after an ellipsis", "[strided_layout]" )
{
  
}

TEST_CASE( "apply_subscripts in strided_layout with a complex subscript should produce the expected result", "[strided_layout]" )
{
   
}

TEST_CASE( "apply_subscripts with no subscripts in a default constructed layout should succeed", "[strided_layout]" )
{
    const strided_layout layout;
    const std::vector<dynamic_subscript> subscripts;

    const auto result = layout.apply_subscripts(xmipp4::make_span(subscripts));

    const strided_layout expected_layout;
    REQUIRE( result == expected_layout );
}

TEST_CASE( "apply_subscripts with an ellipsis in a default constructed layout should succeed", "[strided_layout]" )
{
    const strided_layout layout;
    const std::vector<dynamic_subscript> subscripts = 
    {
        ellipsis(),
    };

    const auto result = layout.apply_subscripts(xmipp4::make_span(subscripts));

    const strided_layout expected_layout;
    REQUIRE( result == expected_layout );
}

TEST_CASE( "apply_subscripts with a new_axis in a default constructed layout should succeed", "[strided_layout]" )
{
    const strided_layout layout;
    const std::vector<dynamic_subscript> subscripts = 
    {
        new_axis(),
    };

    const auto result = layout.apply_subscripts(xmipp4::make_span(subscripts));
    
    const std::array<std::size_t, 1> expected_extents = {1};
    const std::array<std::ptrdiff_t, 1> expected_strides = {0};
    const std::ptrdiff_t expected_offset = 0;
    const auto expected_layout = make_custom_layout(
        expected_extents,
        expected_strides,
        expected_offset
    );

    REQUIRE( result == expected_layout );
}

TEST_CASE( "apply_subscripts in strided_layout with two ellipsis should throw", "[strided_layout]" )
{
    const auto layout = make_test_layout();
    const std::vector<dynamic_subscript> subscripts = 
    {
        ellipsis(),
        new_axis(),
        ellipsis()
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.apply_subscripts(xmipp4::make_span(subscripts)), "Two ellipsis tags were encountered when processing subscripts" );
}

TEST_CASE( "apply_subscripts in strided_layout with too many subscripts should throw", "[strided_layout]" )
{
    const auto layout = make_test_layout();
    const std::vector<dynamic_subscript> subscripts = 
    {
        6,
        2, 
        2,
        odd(),
        even(),
        all(),
        2
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.apply_subscripts(xmipp4::make_span(subscripts)), "An index subscript was encountered, but there are no more axes to process" );
}

TEST_CASE( "apply_subscripts in strided_layout with too many subscripts and ellipsis should throw", "[strided_layout]" )
{
    const auto layout = make_test_layout();
    const std::vector<dynamic_subscript> subscripts = 
    {
        6,
        2, 
        2,
        ellipsis(),
        odd(),
        even(),
        all(),
        2
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.apply_subscripts(xmipp4::make_span(subscripts)), "A slice subscript was encountered, but there are no more axes to process" );
}

TEST_CASE( "apply_subscripts in strided_layout with out of bounds index should throw", "[strided_layout]" )
{
    const auto layout = make_test_layout();
    const std::vector<dynamic_subscript> subscripts = 
    {
        120
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::out_of_range );
}

TEST_CASE( "apply_subscripts in strided_layout with out of bounds index after ellipsis should throw", "[strided_layout]" )
{
    const auto layout = make_test_layout();
    const std::vector<dynamic_subscript> subscripts = 
    {
        ellipsis(),
        8
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::out_of_range );
}

TEST_CASE( "apply_subscripts in strided_layout with out of bounds slice should throw", "[strided_layout]" )
{
    const auto layout = make_test_layout();
    const std::vector<dynamic_subscript> subscripts = 
    {
        make_slice(121)
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::out_of_range );
}

TEST_CASE( "apply_subscripts in strided_layout with out of bounds slice after ellipsis should throw", "[strided_layout]" )
{
    const auto layout = make_test_layout();
    const std::vector<dynamic_subscript> subscripts = 
    {
        ellipsis(),
        make_slice(9)
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::out_of_range );
}

TEST_CASE( "apply_subscripts with an index in a default constructed layout should throw", "[strided_layout]" )
{
    const strided_layout layout;
    const std::vector<dynamic_subscript> subscripts = 
    {
        0
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.apply_subscripts(xmipp4::make_span(subscripts)), "An index subscript was encountered, but there are no more axes to process" );
}

TEST_CASE( "apply_subscripts with an slice in a default constructed layout should throw", "[strided_layout]" )
{
    const strided_layout layout;
    const std::vector<dynamic_subscript> subscripts = 
    {
        make_slice(1)
    };

    REQUIRE_THROWS_AS( layout.apply_subscripts(xmipp4::make_span(subscripts)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.apply_subscripts(xmipp4::make_span(subscripts)), "A slice subscript was encountered, but there are no more axes to process" );
}

TEST_CASE("transpose in strided_layout should reverse the order of its axes", "[strided_layout]")
{

}

TEST_CASE("transpose in a default constructed strided_layout should return another empty layout ", "[strided_layout]")
{

}

TEST_CASE("permute in strided_layout with valid permutation should correctly alter the order of the axes", "[strided_layout]")
{

}

TEST_CASE("permute in a default constructed strided_layout and empty permutation should succeed and return empty permutation", "[strided_layout]")
{

}

TEST_CASE("permute in a default constructed strided_layout and non-empty permutation should throw", "[strided_layout]")
{

}

TEST_CASE("swap_axes in strided_layout should swap the requested axes", "[strided_layout]")
{

}

TEST_CASE("swap_axes in strided_layout should throw when one of the axes is out of bounds", "[strided_layout]")
{
    const auto layout = make_test_layout();

    REQUIRE_THROWS_AS( layout.swap_axes(6, 0), std::out_of_range );
    REQUIRE_THROWS_WITH( layout.swap_axes(6, 0), "Index 6 is out of bounds for extent 6" );
    REQUIRE_THROWS_AS( layout.swap_axes(0, 6), std::out_of_range );
    REQUIRE_THROWS_WITH( layout.swap_axes(6, 0), "Index 6 is out of bounds for extent 6" );
}

TEST_CASE("swap_axes in default constructed strided_layout should always fail", "[strided_layout]")
{
    const strided_layout layout;

    REQUIRE_THROWS_AS( layout.swap_axes(0, 0), std::logic_error );
    REQUIRE_THROWS_WITH( layout.swap_axes(0, 0), "Cannot swap axes on an empty layout" );
    REQUIRE_THROWS_AS( layout.swap_axes(0, 1), std::logic_error );
    REQUIRE_THROWS_WITH( layout.swap_axes(0, 1), "Cannot swap axes on an empty layout" );
}


TEST_CASE("squeeze in strided_layout should remove all axes of extent 1", "[strided_layout]")
{

}

TEST_CASE("squeeze in default constructed strided_layout should return an empty layout", "[strided_layout]")
{

}

TEST_CASE("broadcast_to in strided_layout should fill in the left and promote axes with extent 1", "[strided_layout]")
{

}

TEST_CASE("broadcast_to in strided_layout should throw when the provided extents have less axes than the layout", "[strided_layout]")
{
    const auto layout = make_test_layout();
    const std::vector<std::size_t> target_extents = {56, 24, 1, 10, 8};

    REQUIRE_THROWS_AS( layout.broadcast_to(xmipp4::make_span(target_extents)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.broadcast_to(xmipp4::make_span(target_extents)), "Can not broadcast layout with 6 axes into a shape of 5 dimensions." );
}

TEST_CASE("broadcast_to in strided_layout should throw if an axis is not broadcastable", "[strided_layout]")
{
    const auto layout = make_test_layout();
    const std::vector<std::size_t> target_extents = {120, 55, 24, 1, 10, 8};

    REQUIRE_THROWS_AS( layout.broadcast_to(xmipp4::make_span(target_extents)), std::invalid_argument );
    REQUIRE_THROWS_WITH( layout.broadcast_to(xmipp4::make_span(target_extents)), "Can not broadcast axis of extent 56 into an extent of 55." );
}
