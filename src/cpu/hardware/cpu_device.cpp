// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_device.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/exceptions/invalid_operation_error.hpp>
#include <xmipp4/cpu/hardware/cpu_command_queue.hpp>
#include <xmipp4/cpu/hardware/cpu_event.hpp>
#include <xmipp4/cpu/hardware/cpu_timestamped_event.hpp>

namespace xmipp4
{
namespace hardware
{

void cpu_device::get_memory_resources(
	std::vector<const memory_resource*> &resources
) const
{
	resources = { &get_host_memory_resource() };
}

std::shared_ptr<command_queue> cpu_device::create_command_queue() const
{
	return std::make_shared<cpu_command_queue>();
}

std::shared_ptr<event>
cpu_device::create_event(event_usage_flags usage) const
{
	if (usage.contains(event_usage_flag_bits::timestamp))
	{
		return std::make_shared<cpu_timestamped_event>();
	}

	return std::make_shared<cpu_event>();
}

device::duration_type cpu_device::elapsed_time(
	const event &start, const event &stop
) const
{
	const auto *start_ts = dynamic_cast<const cpu_timestamped_event*>(&start);
	const auto *stop_ts  = dynamic_cast<const cpu_timestamped_event*>(&stop);
	if (start_ts == nullptr || stop_ts == nullptr)
	{
		throw invalid_operation_error(
			"cpu_device::elapsed_time: both events must have been created "
			"with event_usage_flag_bits::timestamp."
		);
	}

	return duration_type(
		stop_ts->get_timestamp_ns() - start_ts->get_timestamp_ns()
	);
}

} // namespace hardware
} // namespace xmipp4
