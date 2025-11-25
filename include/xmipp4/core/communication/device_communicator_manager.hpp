// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_communicator_backend.hpp"
#include "../service_manager.hpp"
#include "../platform/dynamic_shared_object.h"

#include <string>
#include <vector>
#include <memory>

namespace xmipp4 
{
namespace communication
{

class device_communicator;

/**
 * @brief Centralizes all known implementations of the 
 * device_communicator_backend interface.
 * 
 */
class device_communicator_manager final
	: public service_manager
{
public:
	XMIPP4_CORE_API device_communicator_manager();
	device_communicator_manager(const device_communicator_manager &other) = delete;
	device_communicator_manager(device_communicator_manager &&other) = delete;
	XMIPP4_CORE_API ~device_communicator_manager() override;

	device_communicator_manager& 
	operator=(const device_communicator_manager &other) = delete;
	device_communicator_manager& 
	operator=(device_communicator_manager &&other) = delete;

	XMIPP4_CORE_API
	void register_builtin_backends() override;
	
	/**
	 * @brief Register a new backend.
	 * 
	 * @param backend The backend to be registered.
	 * @return true The backend was successfully registered.
	 * @return false The backend could not be registered.
	 */
	XMIPP4_CORE_API
	bool register_backend(std::unique_ptr<device_communicator_backend> backend);

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
	XMIPP4_CORE_API
	void create_world_communicators(
		span<hardware::device*> devices,
		const std::shared_ptr<host_communicator> &host_communicator,
		std::vector<std::shared_ptr<device_communicator>> &out
	) const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	void create_implementation_if_null();

}; 

} // namespace communication
} // namespace xmipp4
