// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <em/image/formats/mrc/mrc_region_window.hpp>

#include <em/image/formats/mrc/mrc_geometry.hpp>
#include <em/image/formats/mrc/mrc_header.hpp>
#include <em/image/formats/mrc/mrc_mode.hpp>

#include <rexlib/em/image/image_transfer_plan.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>

using namespace rexlib;
using namespace rexlib::em;
using namespace rexlib::em::mrc;

namespace
{

// A stack of five 3x4 planes of float32, so one plane is 48 bytes.
REXLIB_CONST_CONSTEXPR std::size_t plane_bytes = 3 * 4 * sizeof(float);

mrc_geometry make_stack_geometry()
{
	mrc_header header;
	header.set_column_count(4);
	header.set_row_count(3);
	header.set_section_count(5);
	header.set_section_sampling(1);
	header.set_space_group(0);
	header.set_mode(mrc_mode::float32);

	return mrc_geometry(header);
}

const std::vector<std::size_t> plane = {3, 4};

void add_plane(image_transfer_plan &regions, std::size_t section)
{
	const std::size_t file_offset[3] = {section, 0, 0};
	const std::size_t array_offset[3] = {0, 0, 0};
	regions.add(make_span(file_offset, 3), make_span(array_offset, 3));
}

} // anonymous namespace

TEST_CASE( "the window of a batch covers the planes it reaches",
	"[mrc_region_window]" )
{
	const auto geometry = make_stack_geometry();

	SECTION( "a batch of no region covers nothing" )
	{
		const image_transfer_plan regions(make_span(plane), 3, 3);

		const auto window = make_region_window(regions, geometry);

		CHECK( window.get_byte_offset() == 0 );
		CHECK( window.get_byte_size() == 0 );
	}

	SECTION( "one plane covers itself alone" )
	{
		image_transfer_plan regions(make_span(plane), 3, 3);
		add_plane(regions, 2);

		const auto window = make_region_window(regions, geometry);

		CHECK( window.get_byte_offset() == 2 * plane_bytes );
		CHECK( window.get_byte_size() == plane_bytes );
	}

	SECTION( "a run of planes starts where the run does" )
	{
		image_transfer_plan regions(make_span(plane), 3, 3);
		add_plane(regions, 1);
		add_plane(regions, 2);
		add_plane(regions, 3);

		const auto window = make_region_window(regions, geometry);

		CHECK( window.get_byte_offset() == plane_bytes );
		CHECK( window.get_byte_size() == 3 * plane_bytes );
	}

	SECTION( "planes out of order and with a gap cover what lies between" )
	{
		image_transfer_plan regions(make_span(plane), 3, 3);
		add_plane(regions, 4);
		add_plane(regions, 1);

		const auto window = make_region_window(regions, geometry);

		CHECK( window.get_byte_offset() == plane_bytes );
		CHECK( window.get_byte_size() == 4 * plane_bytes );
	}

	SECTION( "a region of the whole stack covers all of it" )
	{
		const std::vector<std::size_t> stack = {5, 3, 4};
		image_transfer_plan regions(make_span(stack), 3, 3);
		const std::size_t origin[3] = {0, 0, 0};
		regions.add(make_span(origin, 3), make_span(origin, 3));

		const auto window = make_region_window(regions, geometry);

		CHECK( window.get_byte_offset() == 0 );
		CHECK( window.get_byte_size() == 5 * plane_bytes );
	}
}

TEST_CASE( "the window of a batch stays inside the file",
	"[mrc_region_window]" )
{
	const auto geometry = make_stack_geometry();

	// A region past the end is refused where the batch is resolved. Here it
	// only has to not ask for bytes the mapping does not hold.
	image_transfer_plan regions(make_span(plane), 3, 3);
	add_plane(regions, 9);

	const auto window = make_region_window(regions, geometry);

	CHECK( window.get_byte_offset() <= 5 * plane_bytes );
	CHECK( window.get_byte_offset() + window.get_byte_size() <= 5 * plane_bytes );
}
