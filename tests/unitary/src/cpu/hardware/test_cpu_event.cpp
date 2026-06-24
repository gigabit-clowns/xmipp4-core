// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>

#include <cpu/hardware/cpu_event.hpp>

using namespace xmipp4::hardware;

TEST_CASE( "cpu_event should support host_query, host_wait and device_wait", "[cpu_event]" )
{
	cpu_event evt;
	const auto usage = evt.get_supported_usage();

	REQUIRE( usage.contains(event_usage_flag_bits::host_query) );
	REQUIRE( usage.contains(event_usage_flag_bits::host_wait) );
	REQUIRE( usage.contains(event_usage_flag_bits::device_wait) );
}

TEST_CASE( "cpu_event should not advertise cross_device_wait", "[cpu_event]" )
{
	cpu_event evt;
	REQUIRE_FALSE(
		evt.get_supported_usage().contains(event_usage_flag_bits::cross_device_wait)
	);
}

TEST_CASE( "cpu_event::wait should return immediately", "[cpu_event]" )
{
	cpu_event evt;
	REQUIRE_NOTHROW( evt.wait() );
}

TEST_CASE( "cpu_event::is_signaled should always return true", "[cpu_event]" )
{
	cpu_event evt;
	REQUIRE( evt.is_signaled() );
}
