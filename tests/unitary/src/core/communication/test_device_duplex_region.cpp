// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/core/communication/device_duplex_region.hpp>

#include <xmipp4/core/hardware/buffer.hpp>

using namespace xmipp4;
using namespace xmipp4::communication;

TEST_CASE( "default constructed device_duplex_region should be empty ", "[device_duplex_region]" ) 
{
	const device_duplex_region region;
	REQUIRE( region.get_send_data() == nullptr );
	REQUIRE( region.get_receive_data() == nullptr );
	REQUIRE( region.get_data_type() == numerical_type::unknown );
	REQUIRE( region.get_send_offset() == 0UL );
	REQUIRE( region.get_receive_offset() == 0UL );
	REQUIRE( region.get_count() == 0UL );
}

TEST_CASE( "constructing a device_duplex_region should store its attributes", "[device_duplex_region]" ) 
{
	const auto send_buffer = std::make_shared<hardware::buffer>(
		nullptr, 
		0,
		hardware::get_host_memory_resource(),
		nullptr
	);
	const auto receive_buffer = std::make_shared<hardware::buffer>(
		nullptr, 
		0,
		hardware::get_host_memory_resource(),
		nullptr
	);

	const auto data_type = GENERATE(
		numerical_type::int16,
		numerical_type::float64
	);
	const auto send_offset = GENERATE(2, 3);
	const auto receive_offset = GENERATE(40, 5);
	const auto count = GENERATE(10, 20);

	const device_duplex_region region(
		send_buffer,
		receive_buffer,
		data_type,
		send_offset,
		receive_offset,
		count
	);

	REQUIRE( region.get_send_data() == send_buffer );
	REQUIRE( region.get_receive_data() == receive_buffer );
	REQUIRE( region.get_data_type() == data_type );
	REQUIRE( region.get_send_offset() == send_offset);
	REQUIRE( region.get_receive_offset() == receive_offset );
	REQUIRE( region.get_count() == count );
}

TEST_CASE( "constructing a device_duplex_region from a single buffer should create an aliasing region", "[device_duplex_region]" ) 
{
	const auto send_recv_buffer = std::make_shared<hardware::buffer>(
		nullptr, 
		0,
		hardware::get_host_memory_resource(),
		nullptr
	);

	const auto data_type = GENERATE(
		numerical_type::int16,
		numerical_type::float64
	);
	const auto offset = GENERATE(2, 3);
	const auto count = GENERATE(10, 20);

	const device_duplex_region region(
		send_recv_buffer,
		data_type,
		offset,
		count
	);

	REQUIRE( region.get_send_data() == send_recv_buffer );
	REQUIRE( region.get_receive_data() == send_recv_buffer );
	REQUIRE( region.get_data_type() == data_type );
	REQUIRE( region.get_send_offset() == offset);
	REQUIRE( region.get_receive_offset() == offset );
	REQUIRE( region.get_count() == count );
}
