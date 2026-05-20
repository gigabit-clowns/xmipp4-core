// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_device.hpp>

#include "cpu_command_queue.hpp"
#include "cpu_event.hpp"

#include <xmipp4/core/hardware/memory_resource.hpp>

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
cpu_device::create_event(event_usage_flags /*usage*/) const
{
	return std::make_shared<cpu_event>();
}

} // namespace hardware
} // namespace xmipp4
