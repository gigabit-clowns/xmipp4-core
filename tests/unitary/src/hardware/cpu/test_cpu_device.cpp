// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <xmipp4/core/hardware/cpu/cpu_device.hpp>

using namespace xmipp4::hardware;

TEST_CASE( "creating a device queue on a cpu_device should return null", "[cpu_device]" )
{
	cpu_device dev;
	REQUIRE( dev.create_device_queue() == nullptr );
}

TEST_CASE( "creating a device event on a cpu_device should return null", "[cpu_device]" )
{
	cpu_device dev;
	REQUIRE( dev.create_device_event() == nullptr );
}

TEST_CASE( "creating a device to host event on a cpu_device should return null", "[cpu_device]" )
{
	cpu_device dev;
	REQUIRE( dev.create_device_to_host_event() == nullptr );
}
