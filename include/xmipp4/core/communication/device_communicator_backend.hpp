// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../backend_priority.hpp"
#include "../named_backend.hpp"
#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

#include <string>
#include <memory>
#include <vector>

namespace xmipp4 
{
namespace hardware
{

class device;

} // namespace hardware

namespace communication
{

class device_communicator;
class host_communicator;

/**
 * @brief Abstract class to represent a device_communicator provider.
 * 
 */
class XMIPP4_CORE_API device_communicator_backend
	: public named_backend
{
public:
	device_communicator_backend() = default;
	device_communicator_backend(const device_communicator_backend &other) = delete;
	device_communicator_backend(device_communicator_backend &&other) = delete;
	~device_communicator_backend() override = default;

	device_communicator_backend&
	operator=(const device_communicator_backend &other) = delete;
	device_communicator_backend&
	operator=(device_communicator_backend &&other) = delete;

	/**
	 * @brief Get the suitability of the backend.
	 * 
	 * @param devices The target devices to be used. Neither device may be null.
	 * @return backend_priority The suitability.
	 */
	virtual backend_priority get_suitability(
		span<hardware::device*> devices
	) const = 0;

	/**
	 * @brief Get the world communicators for the requested devices.
	 * 
	 * The world communicator connects all peers together.
	 *
	 * @param node_communicator Host communicator that aids in the construction
	 * of device communicators by communicating different nodes. May be null,
	 * in which case, no attempt to communicate devices across nodes and
	 * processes is made.
	 * @param devices The devices for which the communicators are created.
	 * Neither device may be null.
	 * @param out The resulting device communicators. Must have the same size
	 * as the device span.
	 *  
	 */
	virtual 
	void create_world_communicators(
		host_communicator *node_communicator,
		span<hardware::device*> devices,
		span<std::shared_ptr<device_communicator>> out
	) const = 0;
}; 

} // namespace communication
} // namespace xmipp4
