// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/communication/device_send_region.hpp>

#include <xmipp4/core/hardware/buffer.hpp>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "default constructed device_send_region should be empty ", "[device_send_region]" ) 
{
	const device_send_region region;
	REQUIRE( region.get_data() == nullptr );
	REQUIRE( region.get_data_type() == numerical_type::unknown );
	REQUIRE( region.get_offset() == 0UL );
	REQUIRE( region.get_count() == 0UL );
}

TEST_CASE( "constructing a device_send_region should store its attributes", "[device_send_region]" ) 
{
	const auto buffer = std::make_shared<hardware::buffer>(
		nullptr, 
		0,
		hardware::get_host_memory_resource(),
		nullptr
	);

	const auto data_type = GENERATE(
		numerical_type::int16,
		numerical_type::float64
	);
	const auto count = GENERATE(10, 20);
	const auto offset = GENERATE(4, 8);

	const device_send_region region(
		buffer,
		data_type,
		offset,
		count
	);

	REQUIRE( region.get_data() == buffer );
	REQUIRE( region.get_data_type() == data_type );
	REQUIRE( region.get_offset() == offset );
	REQUIRE( region.get_count() == count );
}
