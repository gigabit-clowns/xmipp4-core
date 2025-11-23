// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "communicator_backend.hpp"
#include "../service_manager.hpp"
#include "../platform/dynamic_shared_object.h"

#include <string>
#include <vector>
#include <memory>

namespace xmipp4 
{
namespace communication
{

class communicator;

/**
 * @brief Centralizes all known implementations of the communicator_backend
 * interface.
 * 
 */
class communicator_manager final
	: public service_manager
{
public:
	XMIPP4_CORE_API communicator_manager();
	communicator_manager(const communicator_manager &other) = delete;
	communicator_manager(communicator_manager &&other) = delete;
	XMIPP4_CORE_API ~communicator_manager() override;

	communicator_manager& 
	operator=(const communicator_manager &other) = delete;
	communicator_manager& 
	operator=(communicator_manager &&other) = delete;

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
	bool register_backend(std::unique_ptr<communicator_backend> backend);

	/**
	 * @brief Get the world communicator from the preferred backend.
	 * 
	 * The world communicator connects all known peers together.
	 * 
	 * @return std::shared_ptr<communicator> The newly created world
	 * communicator.
	 * 
	 */
	XMIPP4_CORE_API
	std::shared_ptr<communicator> create_world_communicator() const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;

	void create_implementation_if_null();
}; 

} // namespace communication
} // namespace xmipp4
