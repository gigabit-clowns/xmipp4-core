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

using namespace xmipp4::multidimensional;

TEST_CASE( "default constructing a array_access_layout_builder should point to a null implementation" )
{
    array_access_layout_builder builder;
    const auto *impl = builder.get_implementation();
    REQUIRE( impl == nullptr );
}

TEST_CASE( "setting the iteration extents in array_access_layout_builder should initialize the implementation" )
{
    array_access_layout_builder builder;

    const std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);

    const auto *impl = builder.get_implementation();
    REQUIRE( impl );
    const auto result = impl->get_extents();
    REQUIRE( std::equal(extents.cbegin(), extents.cend(), result.begin(), result.end()) );
}

TEST_CASE( "setting the iteration extents in array_access_layout_builder with an implementation should throw" )
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

TEST_CASE( "adding the first operand array_access_layout_builder should initialize it" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    auto layout = strided_layout::make_contiguous_layout(xmipp4::make_span(extents));

    builder.add_operand(layout, xmipp4::numerical_type::int32);

    const auto *impl = builder.get_implementation();
    REQUIRE( impl );
    const auto result = impl->get_extents();
    REQUIRE( std::equal(extents.cbegin(), extents.cend(), result.begin(), result.end()) );
}

TEST_CASE( "adding operands to an initialized array_access_layout_builder should broadcast it" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);


    std::vector<std::size_t> layout_extents = {12, 1};
    auto layout = strided_layout::make_contiguous_layout(xmipp4::make_span(extents));
    builder.add_operand(layout, xmipp4::numerical_type::int32);

    const auto *impl = builder.get_implementation();
    REQUIRE( impl );

    REQUIRE( impl->get_number_of_operands() == 1 );
    const auto result_extents = impl->get_extents();
    REQUIRE( std::equal(extents.cbegin(), extents.cend(), result_extents.begin(), result_extents.end()) );
    const auto result_strides = impl->get_strides(0);
    const std::vector<std::ptrdiff_t> expected_strides = { 0, 0, 1, 0 };
    REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides.begin(), result_strides.end()) );
}

TEST_CASE( "adding a non broadcasteable operand to an initialized array_access_layout_builder should throw" )
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);


    std::vector<std::size_t> layout_extents = {8};
    auto layout = strided_layout::make_contiguous_layout(xmipp4::make_span(extents));

	REQUIRE_THROWS_AS( 
        builder.add_operand(layout, xmipp4::numerical_type::int32),
		broadcast_error
	);
}

TEST_CASE("calling build on array_access_layout_builder should reset the implementation")
{
    array_access_layout_builder builder;

    std::vector<std::size_t> extents = {20, 6, 12, 12};
    builder.set_extents(extents);
    builder.build();

    REQUIRE( builder.get_implementation() == nullptr );
}
