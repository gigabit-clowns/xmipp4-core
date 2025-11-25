// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_communicator_backend.hpp"
#include "../named_service_manager.hpp"
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
	: public named_service_manager
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

	XMIPP4_CORE_API
	void enumerate_backends(std::vector<std::string> &names) const override;

	XMIPP4_CORE_API
	device_communicator_backend* 
	get_backend(const std::string &name) const override;

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
	 * @param node_communicator Host communicator that aids in the construction
	 * of device communicators by communicating different nodes. May be null,
	 * in which case, no attempt to communicate devices across nodes and
	 * processes is attempted.
	 * @param devices The devices for which the communicators are created.
	 * Neither device may be null.
	 * @param out The resulting device communicators. Must have the same size
	 * as the device span.
	 *  
	 */
	XMIPP4_CORE_API
	void create_world_communicators(
		host_communicator *node_communicator,
		span<hardware::device*> devices,
		span<std::shared_ptr<device_communicator>> out
	) const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	void create_implementation_if_null();

}; 

} // namespace communication
} // namespace xmipp4
