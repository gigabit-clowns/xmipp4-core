// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "memory_resource_kind.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class memory_heap;
class memory_transfer;
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
	 * @brief Get a non owning pointer to the target device.
	 * 
	 * When the memory resource is allocated in the host memory without
	 * targeting any device (i.e. it is host), it returns nullptr.
	 * 
	 * @return device* Pointer to the device. nullptr if this memory resource
	 * does not target any device.
	 */
	virtual
	device* get_target_device() const noexcept = 0;

	/**
	 * @brief Get the type of the memory resource.
	 * 
	 * @return memory_resource_kind The type of this memory resource.
	 */
	virtual
	memory_resource_kind get_kind() const noexcept = 0;

	/**
	 * @brief Get the maximum alignment parameter allowed for the heap.
	 * 
	 * @return std::size_t The maximum alignment.
	 */
	virtual
	std::size_t get_max_heap_alignment() const noexcept = 0;

	/**
	 * @brief Allocate a chunk of memory to serve smaller allocations.
	 * 
	 * @param size Minimum size requirement for the heap.
	 * @param alignment Minimum alignment requirement for the heap's base
	 * pointer.
	 * 
	 * @return std::shared_ptr<memory_heap> The newly created heap.
	 */
	virtual
	std::shared_ptr<memory_heap> create_memory_heap(
		std::size_t size, 
		std::size_t alignment
	) = 0;
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
memory_resource& get_host_memory_resource() noexcept;

/**
 * @brief Check if a memory resource is accessible by the provided device.
 * 
 * This method uses various heuristics to check if the memory resource is
 * accessible by the provided device.
 * 
 * @param resource The resource to be checked.
 * @param device The device to be checked.
 * @return true If the resource is accessible by the device.
 * @return false If the resource is not accessible by the device.
 */
XMIPP4_CORE_API
bool is_device_accessible(
	const memory_resource &resource, 
	device &device
) noexcept;

} // namespace hardware
} // namespace xmipp4
