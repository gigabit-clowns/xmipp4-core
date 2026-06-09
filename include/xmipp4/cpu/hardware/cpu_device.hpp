// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device.hpp>

namespace xmipp4 
{
namespace hardware
{

class cpu_event;
class cpu_command_queue;
class cpu_memory_resource;

/**
 * @brief Implementation of the @ref device interface to represent the CPU.
 */
class XMIPP4_CORE_API cpu_device final
	: public device
{
public:
	const memory_resource& 
	get_memory_resource(memory_resource_affinity affinity) const override;

	std::shared_ptr<command_queue> create_command_queue() const override;

	std::shared_ptr<event>
	create_event(event_usage_flags usage) const override;
};

} // namespace hardware
} // namespace xmipp4
