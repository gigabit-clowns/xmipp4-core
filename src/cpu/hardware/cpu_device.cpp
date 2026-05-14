// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_device.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/cpu/hardware/cpu_device_queue.hpp>
#include <xmipp4/cpu/hardware/cpu_device_event.hpp>
#include <xmipp4/cpu/hardware/cpu_timestamped_device_event.hpp>

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

std::shared_ptr<device_queue> cpu_device::create_queue() const
{
	return std::make_shared<cpu_device_queue>();
}

std::shared_ptr<device_event> 
cpu_device::create_event(device_event_usage_flags usage) const
{
	if (!usage.contains(device_event_usage_flag_bits::timestamp))
	{
		return std::make_shared<cpu_timestamped_device_event>();
	}
	
	return std::make_shared<cpu_device_event>();
}

} // namespace hardware
} // namespace xmipp4
