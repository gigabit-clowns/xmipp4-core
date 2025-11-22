// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../backend_priority.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class memory_resource;
class memory_transfer;

/**
 * @brief The memory_transfer_backend interface allows to instantiate 
 * memory_transfer objects for specific memory_resource combinations.
 * 
 */
class XMIPP4_CORE_API memory_transfer_backend
{
public:
	memory_transfer_backend() = default;
	memory_transfer_backend(const memory_transfer_backend &other) = default;
	memory_transfer_backend(memory_transfer_backend &&other) = default;
	virtual ~memory_transfer_backend() = default;

	memory_transfer_backend&
	operator=(const memory_transfer_backend &other) = default;
	memory_transfer_backend&
	operator=(memory_transfer_backend &&other) = default;

	/**
	 * @brief Get the backend priority for a memory_transfer between the 
	 * specified pair of memory_resource-s.
	 * 
	 * @param source Source memory resource.
	 * @param destination Destination memory resource.
	 * @return backend_priority The priority for this backend for the requested
	 * transfer.
	 */
	virtual backend_priority get_suitability(
		const memory_resource& source,
		const memory_resource& destination
	) const noexcept = 0;

	/**
	 * @brief Create a transfer object to move data between two memory 
	 * resources.
	 * 
	 * @param source Source memory resource.
	 * @param destination Destination memory resource.
	 * @return std::shared_ptr<memory_transfer> Memory transfer object.
	 * 
	 * @throws std::invalid_argument if get_suitability(source, destination) 
	 * returns unsupported.
	 * 
	 */
	virtual std::shared_ptr<memory_transfer> create_transfer(
		const memory_resource& source,
		const memory_resource& destination
	) const = 0;

};  

} // namespace hardware
} // namespace xmipp4
