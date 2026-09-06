// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <em/image/formats/mrc/mrc_geometry.hpp>

#include <rexlib/core/exceptions/invalid_operation_error.hpp>
#include <rexlib/em/image/exceptions/image_format_error.hpp>

#include <stdexcept>
#include <vector>

using namespace rexlib;
using namespace rexlib::em;
using namespace rexlib::em::mrc;

namespace
{

mrc_header make_header_of(
	std::int32_t columns,
	std::int32_t rows,
	std::int32_t sections,
	std::int32_t section_sampling,
	std::int32_t space_group,
	mrc_mode mode = mrc_mode::float32
)
{
	mrc_header header;
	header.set_column_count(columns);
	header.set_row_count(rows);
	header.set_section_count(sections);
	header.set_section_sampling(section_sampling);
	header.set_space_group(space_group);
	header.set_mode(mode);
	header.set_column_axis(1);
	header.set_row_axis(2);
	header.set_section_axis(3);
	return header;
}

std::vector<std::size_t> extents_of(const mrc_geometry &geometry)
{
	const auto extents = geometry.get_extents();
	return std::vector<std::size_t>(extents.begin(), extents.end());
}

std::vector<std::ptrdiff_t> strides_of(const mrc_geometry &geometry)
{
	const auto strides = geometry.get_strides();
	return std::vector<std::ptrdiff_t>(strides.begin(), strides.end());
}

} // anonymous namespace

TEST_CASE( "the shape of an MRC file follows from its space group",
	"[mrc_geometry]" )
{
	SECTION( "one section and no space group is a single image" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 1, 1, 0));

		REQUIRE( extents_of(geometry) == std::vector<std::size_t>{3, 4} );
		REQUIRE( geometry.get_core_rank() == 2 );
	}

	SECTION( "several sections and no space group is a stack of images" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 5, 1, 0));

		REQUIRE( extents_of(geometry) == std::vector<std::size_t>{5, 3, 4} );
		REQUIRE( geometry.get_core_rank() == 2 );
	}

	SECTION( "a space group of one is a volume" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 5, 5, 1));

		REQUIRE( extents_of(geometry) == std::vector<std::size_t>{5, 3, 4} );
		REQUIRE( geometry.get_core_rank() == 3 );
	}

	SECTION( "a crystallographic space group is a volume too" )
	{
		const mrc_geometry geometry(make_header_of(73, 43, 25, 72, 4));

		REQUIRE( extents_of(geometry) ==
			std::vector<std::size_t>{25, 43, 73} );
		REQUIRE( geometry.get_core_rank() == 3 );
	}

	SECTION( "a space group above four hundred is a stack of volumes" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 12, 4, 401));

		REQUIRE( extents_of(geometry) ==
			std::vector<std::size_t>{3, 4, 3, 4} );
		REQUIRE( geometry.get_core_rank() == 3 );
	}

	SECTION( "the last space group of the range is still a stack" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 12, 4, 630));

		REQUIRE( extents_of(geometry).size() == 4 );
	}

	SECTION( "one past the range is a volume" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 12, 4, 631));

		REQUIRE( extents_of(geometry) ==
			std::vector<std::size_t>{12, 3, 4} );
		REQUIRE( geometry.get_core_rank() == 3 );
	}
}

TEST_CASE( "a crystallographic file does not read its sampling as a depth",
	"[mrc_geometry]" )
{
	// The sampling of EMD-3001, which is unrelated to its section count.
	mrc_header header = make_header_of(73, 43, 25, 72, 4);
	header.set_column_sampling(40);
	header.set_row_sampling(12);

	const mrc_geometry geometry(header);

	REQUIRE( extents_of(geometry) == std::vector<std::size_t>{25, 43, 73} );
	REQUIRE( geometry.get_element_count() == 25 * 43 * 73 );
}

TEST_CASE( "the values of an MRC file are laid out contiguously",
	"[mrc_geometry]" )
{
	SECTION( "a single image counts by rows" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 1, 1, 0));

		REQUIRE( strides_of(geometry) == std::vector<std::ptrdiff_t>{4, 1} );
	}

	SECTION( "a stack counts by sections and rows" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 5, 1, 0));

		REQUIRE( strides_of(geometry) ==
			std::vector<std::ptrdiff_t>{12, 4, 1} );
	}

	SECTION( "a stack of volumes counts by volumes as well" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 12, 4, 401));

		REQUIRE( strides_of(geometry) ==
			std::vector<std::ptrdiff_t>{48, 12, 4, 1} );
	}
}

TEST_CASE( "an MRC file reports where and how much of it holds values",
	"[mrc_geometry]" )
{
	SECTION( "the values follow the main header" )
	{
		const mrc_geometry geometry(make_header_of(4, 3, 5, 1, 0));

		REQUIRE( geometry.get_data_offset() == 1024 );
		REQUIRE( geometry.get_element_count() == 60 );
		REQUIRE( geometry.get_data_size() == 240 );
		REQUIRE( geometry.get_data_type() == numerical_type::float32 );
	}

	SECTION( "an extended header pushes them further" )
	{
		auto header = make_header_of(4, 3, 5, 1, 0);
		header.set_extended_header_size(160);

		const mrc_geometry geometry(header);

		REQUIRE( geometry.get_data_offset() == 1184 );
		REQUIRE( geometry.get_data_size() == 240 );
	}

	SECTION( "the data type resolves mode 0 through the IMOD stamp" )
	{
		auto header = make_header_of(4, 3, 1, 1, 0, mrc_mode::int8);
		header.set_imod_stamp(1146047817);

		const mrc_geometry geometry(header);

		REQUIRE( geometry.get_data_type() == numerical_type::uint8 );
		REQUIRE( geometry.get_data_size() == 12 );
	}
}

TEST_CASE( "values that could not be addressed where they begin are refused",
	"[mrc_geometry]" )
{
	SECTION( "an extended header that misaligns them is refused" )
	{
		auto header = make_header_of(4, 3, 1, 1, 0);
		header.set_extended_header_size(2);

		REQUIRE_THROWS_AS( mrc_geometry(header), image_format_error );
	}

	SECTION( "one that keeps them aligned is not" )
	{
		auto header = make_header_of(4, 3, 1, 1, 0);
		header.set_extended_header_size(4);

		REQUIRE_NOTHROW( mrc_geometry(header) );
	}

	SECTION( "a narrower element tolerates a smaller multiple" )
	{
		auto header = make_header_of(4, 3, 1, 1, 0, mrc_mode::int8);
		header.set_extended_header_size(3);

		REQUIRE_NOTHROW( mrc_geometry(header) );
	}
}

TEST_CASE( "a header is built from the shape a file is created with",
	"[mrc_geometry]" )
{
	const std::vector<std::size_t> image = {3, 4};
	const std::vector<std::size_t> stack = {5, 3, 4};
	const std::vector<std::size_t> volume_stack = {3, 4, 3, 4};

	SECTION( "a single image states one section and no space group" )
	{
		const auto header = make_header(
			make_span(image), 2, numerical_type::float32);

		REQUIRE( header.get_column_count() == 4 );
		REQUIRE( header.get_row_count() == 3 );
		REQUIRE( header.get_section_count() == 1 );
		REQUIRE( header.get_section_sampling() == 1 );
		REQUIRE( header.get_space_group() == 0 );
	}

	SECTION( "a stack of images states a sampling of one" )
	{
		const auto header = make_header(
			make_span(stack), 2, numerical_type::float32);

		REQUIRE( header.get_section_count() == 5 );
		REQUIRE( header.get_section_sampling() == 1 );
		REQUIRE( header.get_space_group() == 0 );
	}

	SECTION( "a volume states its depth as its sampling" )
	{
		const auto header = make_header(
			make_span(stack), 3, numerical_type::float32);

		REQUIRE( header.get_section_count() == 5 );
		REQUIRE( header.get_section_sampling() == 5 );
		REQUIRE( header.get_space_group() == 1 );
	}

	SECTION( "a stack of volumes divides its sections between two axes" )
	{
		const auto header = make_header(
			make_span(volume_stack), 3, numerical_type::float32);

		REQUIRE( header.get_section_count() == 12 );
		REQUIRE( header.get_section_sampling() == 4 );
		REQUIRE( header.get_space_group() == 401 );
	}

	SECTION( "what it does not derive is what a new file carries" )
	{
		const auto header = make_header(
			make_span(image), 2, numerical_type::float32);

		REQUIRE( header.get_column_axis() == 1 );
		REQUIRE( header.get_row_axis() == 2 );
		REQUIRE( header.get_section_axis() == 3 );
		REQUIRE( header.get_version() == 20141 );
		REQUIRE( header.get_cell_angles()[0] == 90.0F );
		REQUIRE( header.get_byte_order() == get_system_byte_order() );
	}

	SECTION( "statistics that were not computed carry their sentinels" )
	{
		const auto header = make_header(
			make_span(image), 2, numerical_type::float32);

		REQUIRE( header.get_data_min() == 0.0F );
		REQUIRE( header.get_data_max() == -1.0F );
		REQUIRE( header.get_data_mean() == -2.0F );
		REQUIRE( header.get_data_rms() == -1.0F );
	}

	SECTION( "unsigned bytes are stamped as such" )
	{
		const auto header = make_header(
			make_span(image), 2, numerical_type::uint8);

		REQUIRE( header.get_mode() == mrc_mode::int8 );
		REQUIRE( header.get_imod_stamp() == 1146047817 );
		REQUIRE_FALSE( holds_signed_bytes(header) );
	}

	SECTION( "signed bytes are not" )
	{
		const auto header = make_header(
			make_span(image), 2, numerical_type::int8);

		REQUIRE( header.get_mode() == mrc_mode::int8 );
		REQUIRE( header.get_imod_stamp() == 0 );
		REQUIRE( holds_signed_bytes(header) );
	}
}

TEST_CASE( "a shape the MRC format cannot hold is refused",
	"[mrc_geometry]" )
{
	const std::vector<std::size_t> image = {3, 4};
	const std::vector<std::size_t> stack = {5, 3, 4};
	const std::vector<std::size_t> volume_stack = {3, 4, 3, 4};
	const std::vector<std::size_t> line = {4};
	const std::vector<std::size_t> too_deep = {2, 3, 4, 3, 4};

	// A core rank that does not name a subset of the extents breaks the
	// contract of image_write_format::open rather than naming a file the MRC
	// format has no shape for, and is refused as such.
	SECTION( "a core rank of zero is refused" )
	{
		REQUIRE_THROWS_AS(
			make_header(make_span(image), 0, numerical_type::float32),
			std::invalid_argument
		);
	}

	SECTION( "a core rank above the rank is refused" )
	{
		REQUIRE_THROWS_AS(
			make_header(make_span(image), 3, numerical_type::float32),
			std::invalid_argument
		);
	}

	SECTION( "a rank of one is refused" )
	{
		REQUIRE_THROWS_AS(
			make_header(make_span(line), 1, numerical_type::float32),
			invalid_operation_error
		);
	}

	SECTION( "a rank above four is refused" )
	{
		REQUIRE_THROWS_AS(
			make_header(make_span(too_deep), 3, numerical_type::float32),
			invalid_operation_error
		);
	}

	SECTION( "a stack of images of images is refused" )
	{
		REQUIRE_THROWS_AS(
			make_header(make_span(volume_stack), 2, numerical_type::float32),
			invalid_operation_error
		);
	}

	SECTION( "a data type the format has no mode for is refused" )
	{
		REQUIRE_THROWS_AS(
			make_header(make_span(stack), 2, numerical_type::float64),
			invalid_operation_error
		);
	}
}

TEST_CASE( "a file created with a shape reports that shape when parsed",
	"[mrc_geometry]" )
{
	const std::vector<std::vector<std::size_t>> shapes = {
		{3, 4}, {5, 3, 4}, {5, 3, 4}, {3, 4, 3, 4}
	};
	const std::size_t core_ranks[] = {2, 2, 3, 3};

	for (std::size_t i = 0; i < shapes.size(); ++i)
	{
		const auto header = make_header(
			make_span(shapes[i]), core_ranks[i], numerical_type::float32);
		const mrc_geometry geometry(header);

		REQUIRE( extents_of(geometry) == shapes[i] );
		REQUIRE( geometry.get_core_rank() == core_ranks[i] );
		REQUIRE( geometry.get_data_type() == numerical_type::float32 );
	}
}
