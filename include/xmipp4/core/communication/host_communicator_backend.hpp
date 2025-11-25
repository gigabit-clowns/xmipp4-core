// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../named_backend.hpp"
#include "../backend_priority.hpp"
#include "../platform/dynamic_shared_object.h"

#include <string>
#include <memory>

namespace xmipp4 
{
namespace communication
{

class host_communicator;

/**
 * @brief Abstract class to represent a host_communicator provider.
 * 
 */
class XMIPP4_CORE_API host_communicator_backend
	: public named_backend
{
public:
	host_communicator_backend() = default;
	host_communicator_backend(const host_communicator_backend &other) = delete;
	host_communicator_backend(host_communicator_backend &&other) = delete;
	~host_communicator_backend() override = default;

	host_communicator_backend&
	operator=(const host_communicator_backend &other) = delete;
	host_communicator_backend&
	operator=(host_communicator_backend &&other) = delete;

	/**
	 * @brief Get the suitability of the backend.
	 * 
	 * @return backend_priority The suitability.
	 */
	virtual backend_priority get_suitability() const = 0;

	/**
	 * @brief Get the world communicator.
	 * 
	 * The world communicator connects all known peers together.
	 * 
	 * @return std::shared_ptr<host_communicator> Reference to the world
	 * host_communicator.
	 */
	virtual 
	std::shared_ptr<host_communicator> create_world_communicator() const = 0;

}; 

} // namespace communication
} // namespace xmipp4
