// SPDX-License-Identifier: GPL-3.0-only

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include <xmipp4/backends/cpu/device.hpp>

#include <backends/cpu/hardware/command_queue.hpp>
#include <backends/cpu/hardware/event.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>
#include <xmipp4/core/hardware/event.hpp>

using namespace xmipp4;
using namespace xmipp4::cpu;

TEST_CASE(
	"cpu::device should expose a single memory resource",
	"[cpu::device]"
)
{
	cpu::device dev;

	const auto affinity = GENERATE(
		memory_resource_affinity::host,
		memory_resource_affinity::device
	);
	REQUIRE(
		&dev.get_memory_resource(affinity) == &get_host_memory_resource()
	);
}

TEST_CASE(
	"cpu::device should create a non-null command_queue",
	"[cpu::device]"
)
{
	cpu::device dev;
	const auto queue = dev.create_command_queue();

	REQUIRE( queue != nullptr );
	REQUIRE( dynamic_cast<cpu::command_queue*>(queue.get()) != nullptr );
}

TEST_CASE(
	"cpu::device should create an independent command_queue on each call",
	"[cpu::device]"
)
{
	cpu::device dev;
	const auto queue_a = dev.create_command_queue();
	const auto queue_b = dev.create_command_queue();

	REQUIRE( queue_a != nullptr );
	REQUIRE( queue_b != nullptr );
}

TEST_CASE(
	"cpu::device should create a non-null event",
	"[cpu::device]"
)
{
	cpu::device dev;
	const auto evt = dev.create_event({});

	REQUIRE( evt != nullptr );
	REQUIRE( dynamic_cast<cpu::event*>(evt.get()) != nullptr );
}

TEST_CASE(
	"cpu::device should ignore the requested event usage flags",
	"[cpu::device]"
)
{
	cpu::device dev;
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
