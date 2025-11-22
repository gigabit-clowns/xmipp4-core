// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/cpu/cpu_device.hpp>

#include <xmipp4/core/hardware/cpu/cpu_device_queue.hpp>
#include <xmipp4/core/hardware/cpu/cpu_event.hpp>
#include <xmipp4/core/hardware/memory_resource.hpp>

namespace xmipp4
{
namespace hardware
{

void cpu_device::enumerate_memory_resources(
	std::vector<memory_resource*> &resources
)
{
	auto &host_memory_resource = get_host_memory_resource();
	resources = { &host_memory_resource };
}

std::shared_ptr<device_queue> cpu_device::create_device_queue()
{
	return std::make_shared<cpu_device_queue>();
}

std::shared_ptr<device_event> cpu_device::create_device_event()
{
	return std::make_shared<cpu_event>();
}

std::shared_ptr<device_to_host_event>
cpu_device::create_device_to_host_event()
{
	return std::make_shared<cpu_event>();
}

} // namespace hardware
} // namespace xmipp4
