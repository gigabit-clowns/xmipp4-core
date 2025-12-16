// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include <vector>

#include "memory_resource_affinity.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class device_event;
class device_to_host_event;
class memory_resource;

/**
 * @brief Abstract representation of a device handle.
 * 
 * This class enables basic interaction functionalities with
 * a hardware device.
 * 
 */
class XMIPP4_CORE_API device
{
public:
	device() noexcept;
	device(const device &other) = delete;
	device(device &&other) = delete;
	virtual ~device();

	device& operator=(const device &other) = delete;
	device& operator=(device &&other) = delete;

	/**
	 * @brief Get the most suitable memory resource for the intended usage.
	 * 
	 * @param affinity The affinity of the memory resource.
	 * @return memory_resource& The memory resource.
	 */
	virtual memory_resource& 
	get_memory_resource(memory_resource_affinity affinity) = 0;

	/**
	 * @brief Create a device queue.
	 * 
	 * @return std::shared_ptr<device_queue> The newly created device queue. 
	 * nullptr if asynchronous execution is not supported.
	 */
	virtual std::shared_ptr<device_queue>
	create_device_queue() = 0;

	/**
	 * @brief Create an intra-device synchronization primitive.
	 * 
	 * @return std::shared_ptr<device_event> The newly created device event. 
	 * nullptr if asynchronous execution is not supported.
	 */
	virtual std::shared_ptr<device_event>
	create_device_event() = 0;

	/**
	 * @brief Create a device to host synchronization primitive.
	 * 
	 * @return std::shared_ptr<device_to_host_event> The newly created 
	 * device_to_host_event. nullptr if asynchronous execution is not supported.
	 */
	virtual std::shared_ptr<device_to_host_event>
	create_device_to_host_event() = 0;
}; 

} // namespace hardware
} // namespace xmipp4
