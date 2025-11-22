// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class buffer;
class device_queue;
class copy_region;
class memory_resource;

/**
 * @brief Represents a copy engine that can transfer data from one
 * memory resource to another (or within the same memory resource).
 * 
 */
class XMIPP4_CORE_API memory_transfer
{
public:
	memory_transfer() = default;
	memory_transfer(const memory_transfer &other) = default;
	memory_transfer(memory_transfer &&other) = default;
	virtual ~memory_transfer() = default;

	memory_transfer& operator=(const memory_transfer &other) = default;
	memory_transfer& operator=(memory_transfer &&other) = default;
	
	/**
	 * @brief Copy the contents of a buffer into another.
	 * 
	 * @param source The buffer from which data is copied. 
	 * @param destination The buffer from which data is copied.
	 * @param regions Set of regions to be copied. All of them must be valid
	 * for both source and destination buffers.
	 * @param queue The queue where the transfer is enqueued. When provided,
	 * it must belong to the same target device as the source and destination 
	 * memory_resource-s. If null, the transfer is performed synchronously.
	 */
	virtual
	void copy(
		const buffer &source, 
		buffer &destination,
		span<const copy_region> regions, 
		device_queue *queue
	) const = 0;

}; 

} // namespace hardware
} // namespace xmipp4
