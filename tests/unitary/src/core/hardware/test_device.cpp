// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/device_properties.hpp>

#include "mock/mock_device.hpp"

using namespace xmipp4::hardware;

TEST_CASE(
	"device should store the properties it was constructed with",
	"[device]"
)
{
	device_properties properties;
	properties.set_name("XMIPP A1234 SuperFast");
	properties.set_type(device_type::gpu);
	properties.set_physical_location("0000:01:00.0");
	properties.set_total_memory_bytes(0xDEADBEEF);
	properties.set_optimal_data_alignment(2048);

	const mock_device dev(properties);

	const auto &stored = dev.get_properties();
	CHECK( stored.get_name() == "XMIPP A1234 SuperFast" );
	CHECK( stored.get_type() == device_type::gpu );
	CHECK( stored.get_physical_location() == "0000:01:00.0" );
	CHECK( stored.get_total_memory_bytes() == 0xDEADBEEF );
	CHECK( stored.get_optimal_data_alignment() == 2048 );
}

TEST_CASE(
	"device should default-construct empty properties",
	"[device]"
)
{
	const mock_device dev;

	const auto &stored = dev.get_properties();
	CHECK( stored.get_name().empty() );
	CHECK( stored.get_type() == device_type::unknown );
	CHECK( stored.get_physical_location().empty() );
	CHECK( stored.get_total_memory_bytes() == 0 );
	CHECK( stored.get_optimal_data_alignment() == 0 );
}

TEST_CASE(
	"device get_properties should return a reference to the stored object",
	"[device]"
)
{
	const mock_device dev(device_properties{});
	REQUIRE( &dev.get_properties() == &dev.get_properties() );
}
