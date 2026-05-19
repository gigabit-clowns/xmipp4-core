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
class cpu_device final
	: public device
{
public:
	cpu_device() = default;
	~cpu_device() override = default;

	void get_memory_resources(
		std::vector<const memory_resource*> &resources
	) const override;

	std::shared_ptr<command_queue> create_command_queue() const override;

	std::shared_ptr<event>
	create_event(event_usage_flags usage) const override;

	duration_type 
	elapsed_time(const event &start, const event &stop) const override;
};

} // namespace hardware
} // namespace xmipp4
