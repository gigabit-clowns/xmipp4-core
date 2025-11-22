// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class buffer;
class device_queue;
class memory_resource;

/**
 * @brief Abstract class for memory allocators.
 * 
 * Instances of memory allocators can be obtained from memory_resource-s. Their
 * purpose is to allocate buffers on the associated memory_resource.
 * 
 */
class XMIPP4_CORE_API memory_allocator
{
public:
	memory_allocator() = default;
	memory_allocator(const memory_allocator &other) = default;
	memory_allocator(memory_allocator &&other) = default;
	virtual ~memory_allocator() = default;

	memory_allocator& operator=(const memory_allocator &other) = default;
	memory_allocator& operator=(memory_allocator &&other) = default;

	/**
	 * @brief Get the associated memory_resource.
	 * 
	 * @return memory_resource& The memory resource where this allocator
	 * allocates buffers.
	 */
	virtual
	memory_resource& get_memory_resource() const noexcept = 0;

	/**
	 * @brief Allocate a buffer of given size and alignment.
	 * 
	 * @param size Size in bytes.
	 * @param alignment Alignment in bytes. Must be a power of two.
	 * @param queue Optional device_queue where the allocation is dispatched.
	 * If provided, the allocation may be asynchronous with respect to the
	 * host. The queue must be associated to the same device as the memory
	 * resource targeted by this allocator. When not provided, the allocation
	 * is synchronous, which means that the buffer is ready to be used when this
	 * method returns.
	 * @return std::shared_ptr<buffer> The newly allocated buffer.
	 */
	virtual
	std::shared_ptr<buffer> allocate(
		std::size_t size, 
		std::size_t alignment, 
		device_queue *queue
	) = 0;

}; 

} // namespace hardware
} // namespace xmipp4
