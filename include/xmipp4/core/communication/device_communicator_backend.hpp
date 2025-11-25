// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../backend_priority.hpp"
#include "../version.hpp"
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
{
public:
	device_communicator_backend() = default;
	device_communicator_backend(const device_communicator_backend &other) = delete;
	device_communicator_backend(device_communicator_backend &&other) = delete;
	virtual ~device_communicator_backend() = default;

	device_communicator_backend&
	operator=(const device_communicator_backend &other) = delete;
	device_communicator_backend&
	operator=(device_communicator_backend &&other) = delete;

	/**
	 * @brief Get the name of the backend.
	 * 
	 * @return std::string The name
	 */
	virtual std::string get_name() const = 0;

	/**
	 * @brief Get the backend version.
	 * 
	 * @return version The version.
	 */
	virtual version get_version() const = 0;

	/**
	 * @brief Get the suitability of the backend.
	 * 
	 * @return backend_priority The suitability.
	 */
	virtual backend_priority get_suitability() const = 0;

	/**
	 * @brief Get the world communicators for the requested devices.
	 * 
	 * The world communicator connects all known peers together.
	 *
	 * @param devices The devices for which the communicators are created.
	 * Neither device may be null.
	 * @param host_communicator Host communicator that aids in the construction
	 * of device communicators.
	 * @param out The resulting device communicators, one for each device.
	 *  
	 */
	virtual 
	void create_world_communicators(
		span<hardware::device*> devices,
		const std::shared_ptr<host_communicator> &host_communicator,
		std::vector<std::shared_ptr<device_communicator>> &out
	) const = 0;

}; 

} // namespace communication
} // namespace xmipp4
