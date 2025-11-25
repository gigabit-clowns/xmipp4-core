// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "host_communicator_backend.hpp"
#include "../named_service_manager.hpp"
#include "../platform/dynamic_shared_object.h"

#include <string>
#include <vector>
#include <memory>

namespace xmipp4 
{
namespace communication
{

class host_communicator;

/**
 * @brief Centralizes all known implementations of the host_communicator_backend
 * interface.
 * 
 */
class host_communicator_manager final
	: public named_service_manager
{
public:
	XMIPP4_CORE_API host_communicator_manager();
	host_communicator_manager(const host_communicator_manager &other) = delete;
	host_communicator_manager(host_communicator_manager &&other) = delete;
	XMIPP4_CORE_API ~host_communicator_manager() override;

	host_communicator_manager& 
	operator=(const host_communicator_manager &other) = delete;
	host_communicator_manager& 
	operator=(host_communicator_manager &&other) = delete;

	XMIPP4_CORE_API
	void register_builtin_backends() override;

	XMIPP4_CORE_API
	void enumerate_backends(std::vector<std::string> &backends) const override;

	XMIPP4_CORE_API
	host_communicator_backend* 
	get_backend(const std::string &name) const override;

	/**
	 * @brief Register a new backend.
	 * 
	 * @param backend The backend to be registered.
	 * @return true The backend was successfully registered.
	 * @return false The backend could not be registered.
	 */
	XMIPP4_CORE_API
	bool register_backend(std::unique_ptr<host_communicator_backend> backend);

	/**
	 * @brief Get the world communicator from the preferred backend.
	 * 
	 * The world communicator connects all known peers together.
	 * 
	 * @return std::shared_ptr<host_communicator> The newly created world
	 * host_communicator.
	 * 
	 */
	XMIPP4_CORE_API
	std::shared_ptr<host_communicator> create_world_communicator() const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	void create_implementation_if_null();

}; 

} // namespace communication
} // namespace xmipp4
