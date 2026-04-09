// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_device.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>

namespace xmipp4
{
namespace hardware
{

void cpu_device::get_memory_resources(
	std::vector<memory_resource*> &resources
)
{
	resources = { &get_host_memory_resource() };
}

std::shared_ptr<device_queue> cpu_device::create_device_queue()
{
	return nullptr;
}

std::shared_ptr<device_event> cpu_device::create_device_event()
{
	return nullptr;
}

std::shared_ptr<device_to_host_event>
cpu_device::create_device_to_host_event()
{
	return nullptr;
}

} // namespace hardware
} // namespace xmipp4
