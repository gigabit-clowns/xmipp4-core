// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/multidimensional/array_access_layout.hpp>

#include <multidimensional/array_access_layout_implementation.hpp>

#include <algorithm>
#include <stdexcept>

using namespace xmipp4::multidimensional;

TEST_CASE( "getting extents on an default constructed array_access_layout should return empty ", "[array_access_layout]" )
{
    array_access_layout layout;
    const auto extents = layout.get_extents();
    REQUIRE( extents.empty() );
}

TEST_CASE( "getting extents on an default constructed array_access_layout should have no operands ", "[array_access_layout]" )
{
    array_access_layout layout;
    REQUIRE( layout.get_number_of_operands() == 0 );
}

TEST_CASE( "getting strides on an default constructed array_access_layout should throw ", "[array_access_layout]" )
{
    array_access_layout layout;
	REQUIRE_THROWS_AS( 
        layout.get_strides(0),
		std::out_of_range
	);
}

TEST_CASE( "getting the offset on an default constructed array_access_layout should throw ", "[array_access_layout]" )
{
    array_access_layout layout;
	REQUIRE_THROWS_AS( 
        layout.get_offset(0),
		std::out_of_range
	);
};

TEST_CASE( "getting extents on an initialized array_access_layout should return its extents ", "[array_access_layout]" )
{
    array_access_layout_implementation::extent_vector_type extents = 
        { 20, 4, 16, 2 };
    auto implementation = 
        std::make_unique<array_access_layout_implementation>(extents);
    array_access_layout layout(std::move(implementation));

    const auto result = layout.get_extents();
    REQUIRE( std::equal(extents.cbegin(), extents.cend(), result.begin(), result.end()) );
}

TEST_CASE( "getting the number of operands on an array_access_layout should return its operand count", "[array_access_layout]" )
{
    array_access_layout_implementation::extent_vector_type extents = 
        { 20, 4, 16, 2 };
    array_access_layout_implementation::stride_vector_type strides = 
        { 1, 20, 80, 1280 };
    auto implementation = 
        std::make_unique<array_access_layout_implementation>(extents);
    implementation->add_operand(extents, strides, 0UL);
    implementation->add_operand(extents, strides, 0UL);
    array_access_layout layout(std::move(implementation));

    REQUIRE( layout.get_number_of_operands() == 2 );
}

TEST_CASE( "getting the strides of an operand on an array_access_layout should return its strides", "[array_access_layout]" )
{
    array_access_layout_implementation::extent_vector_type extents = 
        { 20, 4, 16, 2 };
    array_access_layout_implementation::stride_vector_type strides = 
        { 1, 20, 80, 1280 };
    auto implementation = 
        std::make_unique<array_access_layout_implementation>(extents);
    implementation->add_operand(extents, strides, 0UL);
    array_access_layout layout(std::move(implementation));

    const auto result = layout.get_strides(0);
    REQUIRE( std::equal(strides.cbegin(), strides.cend(), result.begin(), result.end()) );
}

TEST_CASE( "getting the offset of an operand in an array_access_layout should return its offset", "[array_access_layout]" )
{
    array_access_layout_implementation::extent_vector_type extents = 
        { 20, 4, 16, 2 };
    array_access_layout_implementation::stride_vector_type strides = 
        { 1, 20, 80, 1280 };
    auto implementation = 
        std::make_unique<array_access_layout_implementation>(extents);
    implementation->add_operand(extents, strides, 1234UL);
    array_access_layout layout(std::move(implementation));

    REQUIRE( layout.get_offset(0) == 1234UL );
}

TEST_CASE( "getting the strides of an invalid operand on an array_access_layout should throw", "[array_access_layout]" )
{
    array_access_layout_implementation::extent_vector_type extents = 
        { 20, 4, 16, 2 };
    array_access_layout_implementation::stride_vector_type strides = 
        { 1, 20, 80, 1280 };
    auto implementation = 
        std::make_unique<array_access_layout_implementation>(extents);
    implementation->add_operand(extents, strides, 0UL);
    array_access_layout layout(std::move(implementation));

	REQUIRE_THROWS_AS( 
        layout.get_strides(1),
		std::out_of_range
	);
}

TEST_CASE( "getting the offset of an invalid operand in an array_access_layout should throw", "[array_access_layout]" )
{
    array_access_layout_implementation::extent_vector_type extents = 
        { 20, 4, 16, 2 };
    array_access_layout_implementation::stride_vector_type strides = 
        { 1, 20, 80, 1280 };
    auto implementation = 
        std::make_unique<array_access_layout_implementation>(extents);
    implementation->add_operand(extents, strides, 1234UL);
    array_access_layout layout(std::move(implementation));

	REQUIRE_THROWS_AS( 
        layout.get_offset(1),
		std::out_of_range
	);
}
