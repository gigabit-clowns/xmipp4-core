// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device.hpp>

namespace xmipp4 
{
namespace hardware
{

class cpu_event;
class cpu_device_queue;
class cpu_memory_resource;

/**
 * @brief Implementation of the `device` interface to represent the CPU.
 *  
 */
class cpu_device final
	: public device
{
public:
	cpu_device() = default;
	~cpu_device() override = default;

	memory_resource& get_memory_resource(
		memory_resource_affinity affinity
	) override;

	std::shared_ptr<device_queue> create_device_queue() override;

	std::shared_ptr<device_event> create_device_event() override;

	std::shared_ptr<device_to_host_event> 
	create_device_to_host_event() override;
}; 

} // namespace hardware
} // namespace xmipp4
