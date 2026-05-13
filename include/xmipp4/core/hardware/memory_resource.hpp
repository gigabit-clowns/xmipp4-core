// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "memory_resource_kind.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class memory_allocator;
class device;

/**
 * @brief A memory_resource describes a physical memory region.
 * 
 * It can be used to create allocators for this region and query its 
 * properties and affinity to devices.
 * 
 */
class XMIPP4_CORE_API memory_resource
{
public:
	memory_resource() noexcept;
	memory_resource(const memory_resource &other) = delete;
	memory_resource(memory_resource &&other) = delete;
	virtual ~memory_resource();

	memory_resource& operator=(const memory_resource &other) = delete;
	memory_resource& operator=(memory_resource &&other) = delete;

	/**
	 * @brief Get the type of the memory resource.
	 * 
	 * @return memory_resource_kind The type of this memory resource.
	 */
	virtual
	memory_resource_kind get_kind() const noexcept = 0;

	/**
	 * @brief Create a allocator for this resource.
	 * 
	 * @return std::shared_ptr<memory_allocator> The memory allocator.
	 */
	virtual 
	std::shared_ptr<memory_allocator> create_allocator() const = 0;
}; 

/**
 * @brief Get the memory_resource object representing the host memory.
 * 
 * When calling get_kind() on this object, it will return 
 * memory_resource_kind::host and get_target_device() returns nullptr.
 * 
 * @return memory_resource& 
 */
XMIPP4_CORE_API
const memory_resource& get_host_memory_resource() noexcept;

} // namespace hardware
} // namespace xmipp4
