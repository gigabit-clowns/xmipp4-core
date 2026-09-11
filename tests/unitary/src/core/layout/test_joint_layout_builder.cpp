// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_exception.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_range.hpp>

#include <rexlib/core/layout/joint_layout_builder.hpp>

#include <rexlib/core/layout/strided_layout.hpp>
#include <rexlib/core/exceptions/invalid_operation_error.hpp>
#include <rexlib/core/layout/broadcast_error.hpp>
#include <rexlib/core/layout/joint_cursor.hpp>
#include <rexlib/core/layout/joint_layout.hpp>
#include <core/layout/joint_layout_implementation.hpp>

#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace rexlib;

TEST_CASE( "default constructing a joint_layout_builder should point to a null implementation", "[joint_layout_builder]" )
{
	joint_layout_builder builder;
	const auto *impl = builder.get_implementation();
	REQUIRE( impl == nullptr );
}

TEST_CASE( "setting the iteration extents in joint_layout_builder should initialize the implementation", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	const std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));

	const auto *impl = builder.get_implementation();
	REQUIRE( impl );
	const auto result = impl->get_extents();
	REQUIRE( std::equal(extents.cbegin(), extents.cend(), result.begin(), result.end()) );
}

TEST_CASE( "setting the iteration extents in joint_layout_builder with an implementation should throw", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	const std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));

	REQUIRE_THROWS_MATCHES(
		builder.set_extents(rexlib::make_span(extents)),
		rexlib::invalid_operation_error,
		Catch::Matchers::Message("Extents can only be set once and before adding any operand")
	);
}

TEST_CASE( "adding the first operand joint_layout_builder should initialize it", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 6, 12, 12};
	auto layout = strided_layout::make_contiguous_layout(rexlib::make_span(extents));

	builder.add_operand(layout);

	const auto *impl = builder.get_implementation();
	REQUIRE( impl );
	const auto result = impl->get_extents();
	CHECK( std::equal(extents.cbegin(), extents.cend(), result.begin(), result.end()) );
}

TEST_CASE( "Adding a valid operand joint_layout_builder should add it", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 6, 12, 12};
	auto layout = strided_layout::make_contiguous_layout(rexlib::make_span(extents));
	std::vector<std::ptrdiff_t> strides;
	layout.get_strides(strides);
	const auto offset = layout.get_offset();

	builder.set_extents(rexlib::make_span(extents));
	builder.add_operand(layout);

	const auto *impl = builder.get_implementation();
	REQUIRE( impl );
	const auto operand_extents = impl->get_extents();
	CHECK( std::equal(extents.cbegin(), extents.cend(), operand_extents.begin(), operand_extents.end()) );
	REQUIRE( impl->get_number_of_operands() == 1 );
	const auto operand_strides = impl->get_strides(0);
	CHECK( std::equal(strides.cbegin(), strides.cend(), operand_strides.begin(), operand_strides.end()) );
	CHECK( impl->get_offset(0) == offset );
}

TEST_CASE( "Adding an operand with non-broadcastable extents in joint_layout_builder should throw", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents1 = {20, 6, 12, 12};
	std::vector<std::size_t> extents2 = {20, 4, 12, 12};
	auto layout = strided_layout::make_contiguous_layout(rexlib::make_span(extents1));

	builder.set_extents(rexlib::make_span(extents2));

	REQUIRE_THROWS_MATCHES(
		builder.add_operand(layout),
		std::invalid_argument,
		Catch::Matchers::Message(
			"Unable to broadcast extent of 6 into target extent of 4."
		)
	);
}

TEST_CASE( "Adding an operand with a size-1 axis in joint_layout_builder should broadcast it", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	const std::vector<std::size_t> iteration_extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(iteration_extents));

	const std::vector<std::size_t> operand_extents = {20, 1, 12, 12};
	const std::vector<std::ptrdiff_t> operand_strides = { 144, 144, 12, 1 };
	const auto operand_layout = strided_layout::make_custom_layout(
		rexlib::make_span(operand_extents),
		rexlib::make_span(operand_strides)
	);
	builder.add_operand(operand_layout);

	const auto *impl = builder.get_implementation();
	REQUIRE( impl );
	const auto result_extents = impl->get_extents();
	REQUIRE( std::equal(iteration_extents.cbegin(), iteration_extents.cend(), result_extents.begin(), result_extents.end()) );
	REQUIRE( impl->get_number_of_operands() == 1 );
	const std::vector<std::ptrdiff_t> expected_strides = { 144, 0, 12, 1 };
	const auto result_strides = impl->get_strides(0);
	CHECK( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides.begin(), result_strides.end()) );
}

TEST_CASE( "Adding an operand with fewer dimensions in joint_layout_builder should pad with size-1 axes", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	const std::vector<std::size_t> iteration_extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(iteration_extents));

	const std::vector<std::size_t> operand_extents = {12, 12};
	const std::vector<std::ptrdiff_t> operand_strides = { 12, 1 };
	const auto operand_layout = strided_layout::make_custom_layout(
		rexlib::make_span(operand_extents),
		rexlib::make_span(operand_strides)
	);
	builder.add_operand(operand_layout);

	const auto *impl = builder.get_implementation();
	REQUIRE( impl );
	const auto result_extents = impl->get_extents();
	REQUIRE( std::equal(iteration_extents.cbegin(), iteration_extents.cend(), result_extents.begin(), result_extents.end()) );
	REQUIRE( impl->get_number_of_operands() == 1 );
	const std::vector<std::ptrdiff_t> expected_strides = { 0, 0, 12, 1 };
	const auto result_strides = impl->get_strides(0);
	CHECK( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides.begin(), result_strides.end()) );
}

TEST_CASE( "Adding an operand with fewer dimensions and a size-1 axis in joint_layout_builder should broadcast both", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	const std::vector<std::size_t> iteration_extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(iteration_extents));

	const std::vector<std::size_t> operand_extents = {1, 12};
	const std::vector<std::ptrdiff_t> operand_strides = { 12, 1 };
	const auto operand_layout = strided_layout::make_custom_layout(
		rexlib::make_span(operand_extents),
		rexlib::make_span(operand_strides)
	);
	builder.add_operand(operand_layout);

	const auto *impl = builder.get_implementation();
	REQUIRE( impl );
	const auto result_extents = impl->get_extents();
	REQUIRE( std::equal(iteration_extents.cbegin(), iteration_extents.cend(), result_extents.begin(), result_extents.end()) );
	REQUIRE( impl->get_number_of_operands() == 1 );
	const std::vector<std::ptrdiff_t> expected_strides = { 0, 0, 0, 1 };
	const auto result_strides = impl->get_strides(0);
	CHECK( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides.begin(), result_strides.end()) );
}

TEST_CASE( "Adding a broadcastable operand via strides and extents in joint_layout_builder should work", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	const std::vector<std::size_t> iteration_extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(iteration_extents));

	const std::vector<std::size_t> operand_extents = {20, 1, 12, 12};
	const std::vector<std::ptrdiff_t> operand_strides = { 144, 0, 12, 1 };
	const std::ptrdiff_t offset = 7;
	builder.add_operand(
		rexlib::make_span(operand_extents),
		rexlib::make_span(operand_strides),
		offset
	);

	const auto *impl = builder.get_implementation();
	REQUIRE( impl );
	const auto result_extents = impl->get_extents();
	REQUIRE( std::equal(iteration_extents.cbegin(), iteration_extents.cend(), result_extents.begin(), result_extents.end()) );
	REQUIRE( impl->get_number_of_operands() == 1 );
	const std::vector<std::ptrdiff_t> expected_strides = { 144, 0, 12, 1 };
	const auto result_strides = impl->get_strides(0);
	CHECK( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides.begin(), result_strides.end()) );
	CHECK( impl->get_offset(0) == offset );
}

TEST_CASE("build on joint_layout_builder should move the implementation", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));
	const auto *impl = builder.get_implementation();

	auto layout = builder.build();
	REQUIRE( builder.get_implementation() == nullptr );
	REQUIRE( layout.get_implementation() == impl );
}

TEST_CASE("build with enable_reordering on joint_layout_builder should re-order axes such that the first operand appears in column major ordering", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));
	std::vector<std::ptrdiff_t> strides1 = { 864, 144, 12, 1 };
	const auto operand_layout1 = strided_layout::make_custom_layout(rexlib::make_span(extents), rexlib::make_span(strides1));
	builder.add_operand(operand_layout1);
	std::vector<std::ptrdiff_t> strides2 = { 1, 20, 120, 1440 };
	const auto operand_layout2 = strided_layout::make_custom_layout(rexlib::make_span(extents), rexlib::make_span(strides2));
	builder.add_operand(operand_layout2);

	auto layout = builder.build(joint_layout_build_flag_bits::enable_reordering);

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

TEST_CASE("build with enable_coalescing on joint_layout_builder should coalesce contiguous axes", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));
	std::vector<std::ptrdiff_t> strides = { 1, 20, 120, 1440 };
	const auto operand_layout = 
			strided_layout::make_custom_layout(rexlib::make_span(extents), rexlib::make_span(strides));
	builder.add_operand(operand_layout);
	builder.add_operand(operand_layout);

	auto layout = builder.build(joint_layout_build_flag_bits::enable_coalescing);

	const std::vector<std::size_t> expected_extents = { 17280 };
	const std::vector<std::ptrdiff_t> expected_strides = { 1 };
	const auto result_extents = layout.get_extents();
	REQUIRE( std::equal(expected_extents.cbegin(), expected_extents.cend(), result_extents.begin(), result_extents.end()) );
	const auto result_strides1 = layout.get_strides(0);
	REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides1.begin(), result_strides1.end()) );
	const auto result_strides2 = layout.get_strides(1);
	REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides2.begin(), result_strides2.end()) );
}

TEST_CASE("build with enable_coalescing on joint_layout_builder should coalesce phantom axes", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 1, 1, 12};
	builder.set_extents(rexlib::make_span(extents));
	std::vector<std::ptrdiff_t> strides = { 1, 2, 4, 20 };
	const auto operand_layout = 
			strided_layout::make_custom_layout(rexlib::make_span(extents), rexlib::make_span(strides));
	builder.add_operand(operand_layout);
	builder.add_operand(operand_layout);

	auto layout = builder.build(joint_layout_build_flag_bits::enable_coalescing);

	const std::vector<std::size_t> expected_extents = { 240 };
	const std::vector<std::ptrdiff_t> expected_strides = { 1 };
	const auto result_extents = layout.get_extents();
	REQUIRE( std::equal(expected_extents.cbegin(), expected_extents.cend(), result_extents.begin(), result_extents.end()) );
	const auto result_strides1 = layout.get_strides(0);
	REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides1.begin(), result_strides1.end()) );
	const auto result_strides2 = layout.get_strides(1);
	REQUIRE( std::equal(expected_strides.cbegin(), expected_strides.cend(), result_strides2.begin(), result_strides2.end()) );
}

TEST_CASE("build with enable_coalescing on joint_layout_builder should not coalesce non-contiguous axes", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));
	std::vector<std::ptrdiff_t> strides1 = { 1, 40, 240, 2880 }; // Non-contiguous 2nd axis
	const auto operand_layout1 = 
		strided_layout::make_custom_layout(rexlib::make_span(extents), rexlib::make_span(strides1));
	std::vector<std::ptrdiff_t> strides2 = { 1, 20, 120, 2880 }; // Non-contiguous 4th axis
	const auto operand_layout2 = 
		strided_layout::make_custom_layout(rexlib::make_span(extents), rexlib::make_span(strides2));
	builder.add_operand(operand_layout1);
	builder.add_operand(operand_layout2);

	auto layout = builder.build(joint_layout_build_flag_bits::enable_coalescing);

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

TEST_CASE("build on joint_layout_builder without flags should not modify the layout", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));
	std::vector<std::ptrdiff_t> strides = { 1, 20, 120, 1440 };
	const auto operand_layout = 
		strided_layout::make_custom_layout(rexlib::make_span(extents), rexlib::make_span(strides));
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

TEST_CASE( "build with default flags on joint_layout should re-order and coalesce contiguous axes", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));
	std::vector<std::ptrdiff_t> strides = { 1728, 288, 12, 1 };
	const auto operand_layout = strided_layout::make_custom_layout(rexlib::make_span(extents), rexlib::make_span(strides));
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


TEST_CASE( "building a reduce operation in joint_layout_builder should produce expected axis ordering", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	const std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));

	auto result_strides = GENERATE(
		std::vector<std::ptrdiff_t>{ 12, 0, 1, 0 },
		std::vector<std::ptrdiff_t>{ 0, 12, 0, 1 },
		std::vector<std::ptrdiff_t>{ 0, 12, 0, 1 },
		std::vector<std::ptrdiff_t>{ 12, 0, 0, 1 },
		std::vector<std::ptrdiff_t>{ 0, 0, 24, 1 }
	);
	const auto result_layout = 
		strided_layout::make_custom_layout(
			rexlib::make_span(extents), 
			rexlib::make_span(result_strides)
		);
	builder.add_operand(result_layout);

	const std::vector<std::ptrdiff_t> input_strides = { 3456, 288, 12, 1 };
	const auto input_layout = 
		strided_layout::make_custom_layout(
			rexlib::make_span(extents), 
			rexlib::make_span(input_strides)
		);
	builder.add_operand(input_layout);

	auto layout = builder.build();

	const auto compiled_extents = layout.get_extents();
	CHECK( std::equal(extents.crbegin(), extents.crend(), compiled_extents.begin(), compiled_extents.end()) );

	const auto compiled_result_strides = layout.get_strides(0);
	CHECK( std::equal(result_strides.crbegin(), result_strides.crend(), compiled_result_strides.begin(), compiled_result_strides.end()) );

	const auto compiled_input_strides = layout.get_strides(1);
	CHECK( std::equal(input_strides.crbegin(), input_strides.crend(), compiled_input_strides.begin(), compiled_input_strides.end()) );
}

TEST_CASE( "building a reduce operation in joint_layout_builder should simplify when possible", "[joint_layout_builder]" )
{
	joint_layout_builder builder;

	const std::vector<std::size_t> extents = {20, 6, 12, 12};
	builder.set_extents(rexlib::make_span(extents));

	std::vector<std::ptrdiff_t> result_strides = { 0, 0, 12, 1 };
	const auto result_layout = 
		strided_layout::make_custom_layout(
			rexlib::make_span(extents), 
			rexlib::make_span(result_strides)
		);
	builder.add_operand(result_layout);

	const std::vector<std::ptrdiff_t> input_strides = { 864, 144, 12, 1 };
	const auto input_layout = 
		strided_layout::make_custom_layout(
			rexlib::make_span(extents), 
			rexlib::make_span(input_strides)
		);
	builder.add_operand(input_layout);

	auto layout = builder.build();

	const std::vector<std::size_t> expected_extents = {144, 120};
	const auto compiled_extents = layout.get_extents();
	CHECK( std::equal(expected_extents.cbegin(), expected_extents.cend(), compiled_extents.begin(), compiled_extents.end()) );

	const std::vector<std::ptrdiff_t> expected_result_strides = {1, 0};
	const auto compiled_result_strides = layout.get_strides(0);
	CHECK( std::equal(expected_result_strides.cbegin(), expected_result_strides.cend(), compiled_result_strides.begin(), compiled_result_strides.end()) );

	const std::vector<std::ptrdiff_t> expected_input_strides = {1, 144};
	const auto compiled_input_strides = layout.get_strides(1);
	CHECK( std::equal(expected_input_strides.cbegin(), expected_input_strides.cend(), compiled_input_strides.begin(), compiled_input_strides.end()) );
}

namespace
{

using offset_tuple = std::vector<std::ptrdiff_t>;

struct operand_description
{
	std::vector<std::ptrdiff_t> strides;
	std::ptrdiff_t offset;
};

struct layout_description
{
	std::vector<std::size_t> extents;
	std::vector<operand_description> operands;
};

const joint_layout_build_flags reordering_only =
	joint_layout_build_flag_bits::enable_reordering;

const joint_layout_build_flags coalescing_only =
	joint_layout_build_flag_bits::enable_coalescing;

const joint_layout_build_flags reordering_and_coalescing = {
	joint_layout_build_flag_bits::enable_reordering,
	joint_layout_build_flag_bits::enable_coalescing
};

joint_layout build_layout(
	const layout_description &description,
	joint_layout_build_flags flags
)
{
	joint_layout_builder builder;
	builder.set_extents(make_span(description.extents));
	for (const auto &operand : description.operands)
	{
		builder.add_operand(
			make_span(description.extents),
			make_span(operand.strides),
			operand.offset
		);
	}
	return builder.build(flags);
}

std::vector<std::ptrdiff_t> to_vector(span<const std::ptrdiff_t> values)
{
	return std::vector<std::ptrdiff_t>(values.begin(), values.end());
}

std::vector<std::size_t> to_vector(span<const std::size_t> values)
{
	return std::vector<std::size_t>(values.begin(), values.end());
}

/**
 * @brief Every tuple of operand offsets a description holds, sorted.
 *
 * Enumerated from the description rather than through a layout, so that it
 * is what any traversal of a layout built from it has to visit.
 */
std::vector<offset_tuple> enumerate_offsets(
	const layout_description &description
)
{
	const auto &extents = description.extents;

	std::size_t count = 1;
	for (const auto extent : extents)
	{
		count *= extent;
	}

	std::vector<offset_tuple> result;
	std::vector<std::size_t> index(extents.size(), 0);
	for (std::size_t element = 0; element < count; ++element)
	{
		offset_tuple offsets;
		for (const auto &operand : description.operands)
		{
			auto offset = operand.offset;
			for (std::size_t axis = 0; axis < extents.size(); ++axis)
			{
				offset +=
					static_cast<std::ptrdiff_t>(index[axis]) *
					operand.strides[axis];
			}
			offsets.push_back(offset);
		}
		result.push_back(std::move(offsets));

		for (std::size_t axis = 0; axis < extents.size(); ++axis)
		{
			if (++index[axis] < extents[axis])
			{
				break;
			}
			index[axis] = 0;
		}
	}

	std::sort(result.begin(), result.end());
	return result;
}

/**
 * @brief Every tuple of operand offsets a traversal of a layout visits,
 * sorted.
 */
std::vector<offset_tuple> visit_offsets(const joint_layout &layout)
{
	const auto operand_count = layout.get_number_of_operands();

	std::vector<std::ptrdiff_t> inner_strides;
	for (std::size_t operand = 0; operand < operand_count; ++operand)
	{
		const auto strides = layout.get_strides(operand);
		inner_strides.push_back(strides.empty() ? 0 : strides[0]);
	}

	std::vector<offset_tuple> result;
	joint_cursor cursor;
	auto run = layout.iter(cursor);
	while (run > 0)
	{
		const auto offsets = cursor.get_offsets();
		for (std::size_t element = 0; element < run; ++element)
		{
			offset_tuple tuple(operand_count);
			for (std::size_t operand = 0; operand < operand_count; ++operand)
			{
				tuple[operand] =
					offsets[operand] +
					static_cast<std::ptrdiff_t>(element)*inner_strides[operand];
			}
			result.push_back(std::move(tuple));
		}

		run = layout.next(cursor, run);
	}

	std::sort(result.begin(), result.end());
	return result;
}

/**
 * @brief The extent of every axis next to the stride of every operand along
 * it, one entry per axis, sorted.
 */
std::vector<std::vector<std::ptrdiff_t>> axis_columns(
	const joint_layout &layout
)
{
	const auto extents = layout.get_extents();

	std::vector<std::vector<std::ptrdiff_t>> result;
	for (std::size_t axis = 0; axis < extents.size(); ++axis)
	{
		std::vector<std::ptrdiff_t> column = {
			static_cast<std::ptrdiff_t>(extents[axis])
		};
		for (
			std::size_t operand = 0;
			operand < layout.get_number_of_operands();
			++operand
		)
		{
			column.push_back(layout.get_strides(operand)[axis]);
		}
		result.push_back(std::move(column));
	}

	std::sort(result.begin(), result.end());
	return result;
}

/**
 * @brief Strides of a contiguous array whose axes are laid out in a shuffled
 * order, and walked backwards a quarter of the time.
 */
std::vector<std::ptrdiff_t> make_contiguous_strides(
	const std::vector<std::size_t> &extents,
	std::mt19937 &generator
)
{
	std::vector<std::size_t> order(extents.size());
	std::iota(order.begin(), order.end(), std::size_t(0));
	std::shuffle(order.begin(), order.end(), generator);

	std::bernoulli_distribution backwards(0.25);
	const std::ptrdiff_t sign = backwards(generator) ? -1 : 1;

	std::vector<std::ptrdiff_t> strides(extents.size());
	std::ptrdiff_t stride = 1;
	for (auto axis = order.crbegin(); axis != order.crend(); ++axis)
	{
		strides[*axis] = sign*stride;
		stride *= static_cast<std::ptrdiff_t>(
			std::max<std::size_t>(extents[*axis], 1)
		);
	}
	return strides;
}

/**
 * @brief A layout of up to four short axes and up to three operands, some of
 * them contiguous arrays laid out in any order and the rest strided at
 * random, broadcast axes included.
 */
layout_description make_random_description(unsigned seed)
{
	std::mt19937 generator(seed);
	std::uniform_int_distribution<std::size_t> rank_distribution(0, 4);
	std::uniform_int_distribution<std::size_t> extent_distribution(0, 15);
	std::uniform_int_distribution<std::size_t> operand_distribution(1, 3);
	std::uniform_int_distribution<std::ptrdiff_t> stride_distribution(-7, 7);
	std::uniform_int_distribution<std::ptrdiff_t> offset_distribution(0, 99);
	std::bernoulli_distribution contiguous(0.5);

	layout_description description;

	const auto rank = rank_distribution(generator);
	for (std::size_t axis = 0; axis < rank; ++axis)
	{
		// Mostly axes of two to four elements, a quarter of them of one, and
		// now and then an empty one.
		const auto draw = extent_distribution(generator);
		const std::size_t extent =
			draw == 0 ? 0 :
			draw < 5 ? 1 :
			2 + draw % 3;
		description.extents.push_back(extent);
	}

	const auto operand_count = operand_distribution(generator);
	for (std::size_t operand = 0; operand < operand_count; ++operand)
	{
		operand_description result;
		if (contiguous(generator))
		{
			result.strides = make_contiguous_strides(
				description.extents,
				generator
			);
		}
		else
		{
			for (std::size_t axis = 0; axis < rank; ++axis)
			{
				result.strides.push_back(stride_distribution(generator));
			}
		}
		result.offset = offset_distribution(generator);
		description.operands.push_back(std::move(result));
	}

	return description;
}

} // anonymous namespace

TEST_CASE(
	"reordering compares strides too far apart for an int",
	"[joint_layout_builder]"
)
{
	const std::ptrdiff_t far = std::ptrdiff_t(1) << 32;
	const auto strides = GENERATE_COPY(
		std::vector<std::ptrdiff_t>{ 1, far },
		std::vector<std::ptrdiff_t>{ far, 1 }
	);

	const auto layout = build_layout(
		{ { 2, 2 }, { { strides, 0 } } },
		reordering_only
	);

	CHECK( to_vector(layout.get_strides(0)) ==
	       std::vector<std::ptrdiff_t>{ 1, far } );
}

TEST_CASE(
	"reordering walks the shorter of two axes no operand tells apart "
	"innermost, however long they are",
	"[joint_layout_builder]"
)
{
	const std::size_t far = (std::size_t(1) << 32) + 1;
	const auto extents = GENERATE_COPY(
		std::vector<std::size_t>{ 2, far },
		std::vector<std::size_t>{ far, 2 }
	);

	const auto layout = build_layout(
		{ extents, { { { 0, 0 }, 0 } } },
		reordering_only
	);

	CHECK( to_vector(layout.get_extents()) ==
	       std::vector<std::size_t>{ 2, far } );
}

TEST_CASE(
	"reordering lets a later operand order the axes the first one is "
	"broadcast along",
	"[joint_layout_builder]"
)
{
	// The first operand cannot tell its broadcast axis from the other two, so
	// the second one orders it, and the first one orders the rest.
	const layout_description description = {
		{ 2, 3, 4 },
		{
			{ { 1, 0, 6 }, 0 },
			{ { 1, 2, 6 }, 0 }
		}
	};

	const auto layout = build_layout(description, reordering_only);

	CHECK( to_vector(layout.get_strides(0)) ==
	       std::vector<std::ptrdiff_t>{ 1, 0, 6 } );
	CHECK( to_vector(layout.get_strides(1)) ==
	       std::vector<std::ptrdiff_t>{ 1, 2, 6 } );
}

TEST_CASE(
	"reordering steps over an axis nothing tells apart from the others",
	"[joint_layout_builder]"
)
{
	// Equal extents and a single operand broadcast along the middle axis,
	// which therefore compares equal to both of the others.
	const layout_description description = {
		{ 2, 2, 2 },
		{ { { 4, 0, 1 }, 0 } }
	};

	const auto layout = build_layout(description, reordering_only);

	CHECK( to_vector(layout.get_strides(0)) ==
	       std::vector<std::ptrdiff_t>{ 1, 0, 4 } );
}

TEST_CASE(
	"coalescing leaves a single axis of extent one when every axis has "
	"extent one",
	"[joint_layout_builder]"
)
{
	const layout_description description = {
		{ 1, 1, 1 },
		{ { { 5, 7, 9 }, 3 } }
	};

	const auto layout = build_layout(description, coalescing_only);

	CHECK( to_vector(layout.get_extents()) == std::vector<std::size_t>{ 1 } );
	CHECK( layout.get_offset(0) == 3 );
}

TEST_CASE(
	"reordering and coalescing walk a contiguous array as a single axis "
	"whatever order its axes are laid out in",
	"[joint_layout_builder]"
)
{
	const std::vector<std::size_t> array_extents = { 2, 3, 4, 5 };
	const std::vector<std::ptrdiff_t> row_major_strides = { 60, 20, 5, 1 };
	const std::ptrdiff_t sign = GENERATE(1, -1);

	// Axis `a` of the layout is axis order[a] of the array, so every
	// permutation of the order lays the same array out differently.
	std::vector<std::size_t> order = { 0, 1, 2, 3 };
	do
	{
		layout_description description;
		description.operands.push_back({ {}, 0 });
		for (const auto array_axis : order)
		{
			description.extents.push_back(array_extents[array_axis]);
			description.operands[0].strides.push_back(
				sign*row_major_strides[array_axis]
			);
		}

		INFO(
			"axis order " << order[0] << order[1] << order[2] << order[3] <<
			", sign " << sign
		);

		const auto reordered = build_layout(description, reordering_only);
		CHECK( to_vector(reordered.get_extents()) ==
		       std::vector<std::size_t>{ 5, 4, 3, 2 } );
		CHECK( to_vector(reordered.get_strides(0)) ==
		       std::vector<std::ptrdiff_t>{ sign, 5*sign, 20*sign, 60*sign } );

		const auto coalesced =
			build_layout(description, reordering_and_coalescing);
		CHECK( to_vector(coalesced.get_extents()) ==
		       std::vector<std::size_t>{ 120 } );
		CHECK( to_vector(coalesced.get_strides(0)) ==
		       std::vector<std::ptrdiff_t>{ sign } );
	}
	while (std::next_permutation(order.begin(), order.end()));
}

TEST_CASE(
	"a layout built with any flags visits every combination of offsets the "
	"operands hold, and nothing else",
	"[joint_layout_builder]"
)
{
	const auto seed = GENERATE(range(0u, 256u));
	const auto description = make_random_description(seed);
	const auto expected = enumerate_offsets(description);

	const std::vector<joint_layout_build_flags> flag_combinations = {
		{},
		reordering_only,
		coalescing_only,
		reordering_and_coalescing
	};

	for (std::size_t i = 0; i < flag_combinations.size(); ++i)
	{
		INFO( "seed " << seed << ", flag combination " << i );

		const auto layout = build_layout(description, flag_combinations[i]);
		CHECK( layout.compute_element_count() == expected.size() );
		CHECK( visit_offsets(layout) == expected );
	}
}

TEST_CASE(
	"reordering only permutes the axes, each keeping its extent and the "
	"stride of every operand along it",
	"[joint_layout_builder]"
)
{
	const auto seed = GENERATE(range(0u, 256u));
	const auto description = make_random_description(seed);
	INFO( "seed " << seed );

	const auto verbatim = build_layout(description, {});
	const auto reordered = build_layout(description, reordering_only);

	CHECK( axis_columns(reordered) == axis_columns(verbatim) );
}

TEST_CASE(
	"reordering walks a first operand with no broadcast axis from its "
	"smallest stride to its largest",
	"[joint_layout_builder]"
)
{
	const auto seed = GENERATE(range(0u, 256u));
	auto description = make_random_description(seed);
	for (auto &stride : description.operands[0].strides)
	{
		if (stride == 0)
		{
			stride = 1;
		}
	}
	INFO( "seed " << seed );

	const auto reordered = build_layout(description, reordering_only);

	const auto strides = reordered.get_strides(0);
	for (std::size_t axis = 1; axis < strides.size(); ++axis)
	{
		INFO( "axis " << axis );
		CHECK( std::abs(strides[axis - 1]) <= std::abs(strides[axis]) );
	}
}

TEST_CASE(
	"coalescing leaves no axis of extent one and no two adjacent axes every "
	"operand could walk as one",
	"[joint_layout_builder]"
)
{
	const auto seed = GENERATE(range(0u, 256u));
	const auto description = make_random_description(seed);
	INFO( "seed " << seed );

	const auto coalesced = build_layout(description, coalescing_only);
	const auto extents = coalesced.get_extents();

	if (extents.size() > 1)
	{
		for (const auto extent : extents)
		{
			CHECK( extent != 1 );
		}
	}

	for (std::size_t axis = 1; axis < extents.size(); ++axis)
	{
		INFO( "axes " << axis - 1 << " and " << axis );

		bool mergeable = true;
		for (
			std::size_t operand = 0;
			operand < coalesced.get_number_of_operands();
			++operand
		)
		{
			const auto strides = coalesced.get_strides(operand);
			const auto walked =
				static_cast<std::ptrdiff_t>(extents[axis - 1]) *
				strides[axis - 1];
			mergeable = mergeable && walked == strides[axis];
		}
		CHECK( !mergeable );
	}
}
