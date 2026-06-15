// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_device.hpp>

#include "cpu_command_queue.hpp"
#include "cpu_event.hpp"

#include <core/hardware/host_memory/host_memory_resource.hpp>

namespace xmipp4
{
namespace hardware
{

cpu_device::cpu_device(device_properties properties) noexcept
	: device(std::move(properties))
{
}

cpu_device::~cpu_device() = default;

const memory_resource&
cpu_device::get_memory_resource(memory_resource_affinity /*affinity*/) const
{
	return host_memory_resource::get();
}

std::shared_ptr<command_queue> cpu_device::get_default_queue() const
{
	return cpu_command_queue::create(); // Singleton
}

std::shared_ptr<command_queue> cpu_device::create_command_queue() const
{
	return cpu_command_queue::create();
}

std::shared_ptr<event>
cpu_device::create_event(event_usage_flags /*usage*/) const
{
	return std::make_shared<cpu_event>();
}

} // namespace hardware
} // namespace xmipp4
