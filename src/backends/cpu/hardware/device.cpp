// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/backends/cpu/device.hpp>

#include "command_queue.hpp"
#include "event.hpp"

#include <core/hardware/host_memory/host_memory_resource.hpp>

namespace xmipp4
{
namespace cpu
{

device::device() noexcept = default;

device::~device() = default;

const memory_resource&
device::get_memory_resource(memory_resource_affinity /*affinity*/) const
{
	return host_memory_resource::get();
}

std::shared_ptr<xmipp4::command_queue> device::create_command_queue() const
{
	return command_queue::create();
}

std::shared_ptr<xmipp4::event>
device::create_event(event_usage_flags /*usage*/) const
{
	return std::make_shared<event>();
}

} // namespace cpu
} // namespace xmipp4
