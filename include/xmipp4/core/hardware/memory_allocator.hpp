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
	memory_allocator() noexcept;
	memory_allocator(const memory_allocator &other) = delete;
	memory_allocator(memory_allocator &&other) = delete;
	virtual ~memory_allocator();

	memory_allocator& operator=(const memory_allocator &other) = delete;
	memory_allocator& operator=(memory_allocator &&other) = delete;

	/**
	 * @brief Get the associated memory_resource.
	 * 
	 * @return memory_resource& The memory resource where this allocator
	 * allocates buffers.
	 */
	virtual
	const memory_resource& get_memory_resource() const noexcept = 0;

	/**
	 * @brief Get the maximum alignment allowed for buffers.
	 * 
	 * @return std::size_t The maximum alignment in bytes.
	 */
	virtual std::size_t get_max_alignment() const noexcept = 0;

	/**
	 * @brief Allocate a buffer of given size and alignment.
	 * 
	 * @param size Size in bytes.
	 * @param alignment Alignment in bytes. Must be a power of two and smaller
	 * or equal than the value returned by get_max_alignment().
	 * @param queue_hint Optional device_queue where the allocation is will be 
	 * first used.
	 * @return std::shared_ptr<buffer> The newly allocated buffer.
	 */
	virtual
	std::shared_ptr<buffer> allocate(
		std::size_t size, 
		std::size_t alignment, 
		device_queue *queue_hint = nullptr
	) = 0;

	/**
	 * @brief Acknowledge that a buffer is being used in a certain 
	 * device_queue.
	 * 
	 * Due to the asynchronous nature of the device_queue-s, the buffer may
	 * be needed after its destruction on the application code. This function
	 * acknowledges the allocator that a particular buffer is being used at
	 * a queue in order to defer its release until all pending tasks have been 
	 * completed.
	 * 
	 * @param buffer The buffer being used. Must be have been allocated by 
	 * this allocator.
	 * @param queue The queue where the buffer is being used.
	 */
	virtual
	void record_use(const buffer &buffer, device_queue &queue) = 0;
}; 

} // namespace hardware
} // namespace xmipp4
