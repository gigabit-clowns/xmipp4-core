// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include <vector>

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
	device() = default;
	device(const device &other) = delete;
	device(device &&other) = delete;
	virtual ~device() = default;

	device& operator=(const device &other) = delete;
	device& operator=(device &&other) = delete;

	/**
	 * @brief Get a memory resource object that is local to the device.
	 * 
	 * The device-local memory resource is optimal for operating in the device.
	 * 
	 * @return memory_resource& The device-local memory resource.
	 */
	virtual memory_resource& get_device_local_memory_resource() noexcept = 0;

	/**
	 * @brief Get a memory resource object that is host accessible.
	 * 
	 * The host accessible memory resource is optimal to transfer data from
	 * the host to the device. In unified architectures this may alias the
	 * device local memory resource.
	 * 
	 * @return memory_resource& The host accessible memory resource.
	 */
	virtual memory_resource& get_host_accessible_memory_resource() noexcept = 0;

	/**
	 * @brief Create a device queue.
	 * 
	 * @return std::shared_ptr<device_queue> The created device queue.
	 */
	virtual std::shared_ptr<device_queue>
	create_device_queue() = 0;

	/**
	 * @brief Create an intra-device synchronization primitive.
	 * 
	 * @return std::shared_ptr<device_event> The created device event.
	 */
	virtual std::shared_ptr<device_event>
	create_device_event() = 0;

	/**
	 * @brief Create a device to host synchronization primitive.
	 * 
	 * @return std::shared_ptr<device_to_host_event> The created 
	 * device_to_host_event.
	 */
	virtual std::shared_ptr<device_to_host_event>
	create_device_to_host_event() = 0;
}; 

} // namespace hardware
} // namespace xmipp4
