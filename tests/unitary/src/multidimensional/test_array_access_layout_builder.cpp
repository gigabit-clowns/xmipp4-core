// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>

#include <xmipp4/core/multidimensional/array_access_layout_builder.hpp>

#include <xmipp4/core/multidimensional/strided_layout.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/core/multidimensional/broadcast_error.hpp>
#include <xmipp4/core/multidimensional/array_access_layout.hpp>
#include <multidimensional/array_access_layout_implementation.hpp>

#include <algorithm>
#include <iostream>

using namespace xmipp4::multidimensional;

TEST_CASE( "default constructing a array_access_layout_builder should point to a null implementation", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;
    const auto *impl = builder.get_implementation();
    REQUIRE( impl == nullptr );
}

TEST_CASE( "setting the iteration extents in array_access_layout_builder should initialize the implementation", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    const std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);

    const auto *impl = builder.get_implementation();
    REQUIRE( impl );
    const auto result = impl->get_extents();
    REQUIRE( std::equal(extents.cbegin(), extents.cend(), result.begin(), result.end()) );
}

TEST_CASE( "setting the iteration extents in array_access_layout_builder with an implementation should throw", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    const std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);

	REQUIRE_THROWS_MATCHES( 
        builder.set_extents(extents),
		xmipp4::invalid_operation_error,
		Catch::Matchers::Message("Extents can only be set once and before adding any operand")
	);
}

TEST_CASE( "adding the first operand array_access_layout_builder should initialize it", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    auto layout = strided_layout::make_contiguous_layout(xmipp4::make_span(extents));

    builder.add_operand(layout);

    const auto *impl = builder.get_implementation();
    REQUIRE( impl );
    const auto result = impl->get_extents();
    REQUIRE( std::equal(extents.cbegin(), extents.cend(), result.begin(), result.end()) );
}

TEST_CASE( "adding operands to an initialized array_access_layout_builder should broadcast it", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);


    std::vector<std::size_t> layout_extents = {12, 1};
    auto layout = strided_layout::make_contiguous_layout(xmipp4::make_span(layout_extents));
    builder.add_operand(layout);

    const auto *impl = builder.get_implementation();
    REQUIRE( impl );

    REQUIRE( impl->get_number_of_operands() == 1 );
    const auto result_extents = impl->get_extents();
    REQUIRE( std::equal(extents.cbegin(), extents.cend(), result_extents.begin(), result_extents.end()) );
    const auto result_strides = impl->get_strides(0);
    const std::vector<std::ptrdiff_t> expected_strides = { 0, 0, 1, 0 };
    REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides.begin(), result_strides.end()) );
}

TEST_CASE( "adding a non broadcastable operand to an initialized array_access_layout_builder should throw", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);


    std::vector<std::size_t> layout_extents = {8};
    auto layout = strided_layout::make_contiguous_layout(xmipp4::make_span(layout_extents));

	REQUIRE_THROWS_AS( 
        builder.add_operand(layout),
		broadcast_error
	);
}

TEST_CASE("build on array_access_layout_builder should move the implementation", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);
    
    const auto *impl = builder.get_implementation();
    auto access_layout = builder.build();

    REQUIRE( builder.get_implementation() == nullptr );
    REQUIRE( access_layout.get_implementation() == impl );
}

TEST_CASE("build with enable_reordering on array_access_layout_builder should re-order axes such that the first operand appears in column major ordering", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);
    std::vector<std::ptrdiff_t> strides1 = { 864, 144, 12, 1 };
    const auto operand_layout1 = strided_layout::make_custom_layout(xmipp4::make_span(extents), xmipp4::make_span(strides1));
    builder.add_operand(operand_layout1);
    std::vector<std::ptrdiff_t> strides2 = { 1, 20, 120, 1440 };
    const auto operand_layout2 = strided_layout::make_custom_layout(xmipp4::make_span(extents), xmipp4::make_span(strides2));
    builder.add_operand(operand_layout2);

    auto layout = builder.build(array_access_layout_build_flag_bits::enable_reordering);

    const std::vector<std::size_t> expected_extents = {12, 12, 6, 20};
    const std::vector<std::ptrdiff_t> expected_strides1 = { 1, 12, 144, 864 };
    const std::vector<std::ptrdiff_t> expected_strides2 = { 1440, 120, 20, 1 };
    const auto result_extents = layout.get_extents();
    REQUIRE( std::equal(expected_extents.cbegin(), expected_extents.cend(), result_extents.begin(), result_extents.end()) );
    const auto result_strides1 = layout.get_strides(0);
    REQUIRE( std::equal(expected_strides1.cbegin(), expected_strides1.cend(), result_strides1.begin(), result_strides1.end()) );
    const auto result_strides2 = layout.get_strides(1);
    REQUIRE( std::equal(expected_strides2.cbegin(), expected_strides2.cend(), result_strides2.begin(), result_strides2.end()) );
}

TEST_CASE("build with enable_coalescing on array_access_layout_builder should coalesce contiguous axes", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);
    std::vector<std::ptrdiff_t> strides = { 1, 20, 120, 1440 };
    const auto operand_layout = 
        strided_layout::make_custom_layout(xmipp4::make_span(extents), xmipp4::make_span(strides));
    builder.add_operand(operand_layout);
    builder.add_operand(operand_layout);

    auto layout = builder.build(array_access_layout_build_flag_bits::enable_coalescing);

    const std::vector<std::size_t> expected_extents = { 17280 };
    const std::vector<std::ptrdiff_t> expected_strides = { 1 };
    const auto result_extents = layout.get_extents();
    REQUIRE( std::equal(expected_extents.cbegin(), expected_extents.cend(), result_extents.begin(), result_extents.end()) );
    const auto result_strides1 = layout.get_strides(0);
    REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides1.begin(), result_strides1.end()) );
    const auto result_strides2 = layout.get_strides(1);
    REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides2.begin(), result_strides2.end()) );
}

TEST_CASE("build with enable_coalescing on array_access_layout_builder should coalesce phantom axes", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 1, 1, 12};
    builder.set_extents(extents);
    std::vector<std::ptrdiff_t> strides = { 1, 2, 4, 20 };
    const auto operand_layout = 
        strided_layout::make_custom_layout(xmipp4::make_span(extents), xmipp4::make_span(strides));
    builder.add_operand(operand_layout);
    builder.add_operand(operand_layout);

    auto layout = builder.build(array_access_layout_build_flag_bits::enable_coalescing);

    const std::vector<std::size_t> expected_extents = { 240 };
    const std::vector<std::ptrdiff_t> expected_strides = { 1 };
    const auto result_extents = layout.get_extents();
    REQUIRE( std::equal(expected_extents.cbegin(), expected_extents.cend(), result_extents.begin(), result_extents.end()) );
    const auto result_strides1 = layout.get_strides(0);
    REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides1.begin(), result_strides1.end()) );
    const auto result_strides2 = layout.get_strides(1);
    REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides2.begin(), result_strides2.end()) );
}


TEST_CASE("build with enable_coalescing on array_access_layout_builder should not coalesce non-contiguous axes", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);
    std::vector<std::ptrdiff_t> strides1 = { 1, 40, 240, 2880 }; // Non-contiguous 2nd axis
    const auto operand_layout1 = 
        strided_layout::make_custom_layout(xmipp4::make_span(extents), xmipp4::make_span(strides1));
    std::vector<std::ptrdiff_t> strides2 = { 1, 20, 120, 2880 }; // Non-contiguous 4th axis
    const auto operand_layout2 = 
        strided_layout::make_custom_layout(xmipp4::make_span(extents), xmipp4::make_span(strides2));
    builder.add_operand(operand_layout1);
    builder.add_operand(operand_layout2);

    auto layout = builder.build(array_access_layout_build_flag_bits::enable_coalescing);

    const std::vector<std::size_t> expected_extents = { 20, 72, 12 };
    const std::vector<std::ptrdiff_t> expected_strides1 = { 1, 40, 2880 };
    const std::vector<std::ptrdiff_t> expected_strides2 = { 1, 20, 2880 };
    const auto result_extents = layout.get_extents();
    REQUIRE( std::equal(expected_extents.cbegin(), expected_extents.cend(), result_extents.begin(), result_extents.end()) );
    const auto result_strides1 = layout.get_strides(0);
    REQUIRE( std::equal(expected_strides1.cbegin(), expected_strides1.cend(), result_strides1.begin(), result_strides1.end()) );
    const auto result_strides2 = layout.get_strides(1);
    REQUIRE( std::equal(expected_strides2.cbegin(), expected_strides2.cend(), result_strides2.begin(), result_strides2.end()) );
}

TEST_CASE("build on array_access_layout_builder without flags should not modify the layout", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);
    std::vector<std::ptrdiff_t> strides = { 1, 20, 120, 1440 };
    const auto operand_layout = 
        strided_layout::make_custom_layout(xmipp4::make_span(extents), xmipp4::make_span(strides));
    builder.add_operand(operand_layout);
    builder.add_operand(operand_layout);

    auto layout = builder.build({});

    const auto result_extents = layout.get_extents();
    REQUIRE( std::equal(extents.cbegin(), extents.cend(), result_extents.begin(), result_extents.end()) );
    const auto result_strides1 = layout.get_strides(0);
    REQUIRE( std::equal(strides.cbegin(), strides.cend(), result_strides1.begin(), result_strides1.end()) );
    const auto result_strides2 = layout.get_strides(1);
    REQUIRE( std::equal(strides.cbegin(), strides.cend(), result_strides2.begin(), result_strides2.end()) );
}

TEST_CASE( "build with default flags on array_access_layout should re-order and coalesce contiguous axes", "[array_access_layout_builder]" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);
    std::vector<std::ptrdiff_t> strides = { 1728, 288, 12, 1 };
    const auto operand_layout = strided_layout::make_custom_layout(xmipp4::make_span(extents), xmipp4::make_span(strides));
    builder.add_operand(operand_layout);
    builder.add_operand(operand_layout);

    auto layout = builder.build();

    const std::vector<std::size_t> expected_extents = { 144, 120 };
    const std::vector<std::ptrdiff_t> expected_strides = { 1, 288 };
    const auto result_extents = layout.get_extents();
    REQUIRE( std::equal(expected_extents.cbegin(), expected_extents.cend(), result_extents.begin(), result_extents.end()) );
    const auto result_strides1 = layout.get_strides(0);
    REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides1.begin(), result_strides1.end()) );
    const auto result_strides2 = layout.get_strides(1);
    REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides2.begin(), result_strides2.end()) );
}
