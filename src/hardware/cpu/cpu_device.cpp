// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/cpu/cpu_device.hpp>

#include <xmipp4/core/hardware/memory_resource.hpp>

namespace xmipp4
{
namespace hardware
{

memory_resource& cpu_device::get_device_local_memory_resource() noexcept
{
	return get_host_memory_resource();
}

memory_resource& cpu_device::get_host_accessible_memory_resource() noexcept
{
	return get_host_memory_resource();
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
