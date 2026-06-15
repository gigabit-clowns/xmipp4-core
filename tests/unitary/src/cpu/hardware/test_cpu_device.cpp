// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/cpu/hardware/cpu_device.hpp>

#include <cpu/hardware/cpu_command_queue.hpp>
#include <cpu/hardware/cpu_event.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>
#include <xmipp4/core/hardware/event.hpp>

using namespace xmipp4::hardware;

TEST_CASE(
	"cpu_device should expose a single memory resource",
	"[cpu_device]"
)
{
	cpu_device dev(device_properties{});

	const auto affinity = GENERATE(
		memory_resource_affinity::host,
		memory_resource_affinity::device
	);
	REQUIRE(
		&dev.get_memory_resource(affinity) == &get_host_memory_resource()
	);
}

TEST_CASE(
	"cpu_device should create a non-null cpu_command_queue",
	"[cpu_device]"
)
{
	cpu_device dev(device_properties{});
	const auto queue = dev.create_command_queue();

	REQUIRE( queue != nullptr );
	REQUIRE( dynamic_cast<cpu_command_queue*>(queue.get()) != nullptr );
}

TEST_CASE(
	"cpu_device should create an independent cpu_command_queue on each call",
	"[cpu_device]"
)
{
	cpu_device dev(device_properties{});
	const auto queue_a = dev.create_command_queue();
	const auto queue_b = dev.create_command_queue();

	REQUIRE( queue_a != nullptr );
	REQUIRE( queue_b != nullptr );
	REQUIRE( queue_a != queue_b );
}

TEST_CASE(
	"cpu_device should create a non-null cpu_event",
	"[cpu_device]"
)
{
	cpu_device dev(device_properties{});
	const auto evt = dev.create_event({});

	REQUIRE( evt != nullptr );
	REQUIRE( dynamic_cast<cpu_event*>(evt.get()) != nullptr );
}

TEST_CASE(
	"cpu_device should expose the properties it was constructed with",
	"[cpu_device]"
)
{
	device_properties properties;
	properties.set_name("Test CPU");
	properties.set_type(device_type::cpu);
	properties.set_total_memory_bytes(0xCAFE);
	properties.set_optimal_data_alignment(64);

	cpu_device dev(properties);

	const auto &stored = dev.get_properties();
	REQUIRE( stored.get_name() == "Test CPU" );
	REQUIRE( stored.get_type() == device_type::cpu );
	REQUIRE( stored.get_total_memory_bytes() == 0xCAFE );
	REQUIRE( stored.get_optimal_data_alignment() == 64 );
}

TEST_CASE(
	"cpu_device should ignore the requested event usage flags",
	"[cpu_device]"
)
{
	cpu_device dev(device_properties{});
	const auto evt = dev.create_event({
		event_usage_flag_bits::host_query,
		event_usage_flag_bits::host_wait,
		event_usage_flag_bits::device_wait
	});

	REQUIRE( evt != nullptr );
	const auto usage = evt->get_supported_usage();
	REQUIRE( usage.contains(event_usage_flag_bits::host_query) );
	REQUIRE( usage.contains(event_usage_flag_bits::host_wait) );
	REQUIRE( usage.contains(event_usage_flag_bits::device_wait) );
}
