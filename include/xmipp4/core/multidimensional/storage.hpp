// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class buffer;
class memory_resource;
class device_queue;
	
} // namespace hardware

namespace multidimensional
{

class storage
{
public:
	XMIPP4_CORE_API storage() noexcept;

	XMIPP4_CORE_API
	explicit storage(std::shared_ptr<hardware::buffer> buffer) noexcept;

	storage(const storage &other) = delete;

	XMIPP4_CORE_API
	storage(storage &&other) noexcept;

	XMIPP4_CORE_API
	~storage();

	storage& operator=(const storage &other) = delete;

	XMIPP4_CORE_API
	storage& operator=(storage &&other) noexcept;

	/**
	 * @brief Obtain an alias to this data.
	 * 
	 * @return storage The alias.
	 */
	storage view() noexcept;

	/**
	 * @brief Get the underlying buffer.
	 * 
	 * @return hardware::buffer* The buffer. nullptr if none.
	 */
	hardware::buffer* get_buffer() noexcept;

	/**
	 * @brief Get the underlying buffer.
	 * 
	 * @return const hardware::buffer* The buffer. nullptr if none.
	 */
	const hardware::buffer* get_buffer() const noexcept;

	/**
	 * @brief Get a host accessible pointer to the data.
	 * 
	 * This method only returns a pointer if the data is accessible by the 
	 * host, i.e., if the kind of the underlying memory_resource is one of:
	 * device_mapped, host_staging, unified or managed.
	 * 
	 * @return void* Pointer to the data. nullptr if no buffer is associated or
	 * the buffer is not host accessible.
	 */
	XMIPP4_CORE_API 
	void* get_host_ptr() noexcept;

	/**
	 * @brief Get a host accessible pointer to the data.
	 * 
	 * This method only returns a pointer if the data is accessible by the 
	 * host, i.e., if the kind of the underlying memory_resource is one of:
	 * device_mapped, host_staging, unified or managed.
	 * 
	 * @return const void* Pointer to the data. nullptr if no buffer is 
	 * associated or the buffer is not host accessible.
	 */
	XMIPP4_CORE_API 
	const void* get_host_ptr() const noexcept;

	/**
	 * @brief Get the size in bytes for this buffer.
	 * 
	 * @return std::size_t Size in bytes. 0 if not buffer is associated.
	 */
	XMIPP4_CORE_API 
	std::size_t get_size() const noexcept;

	/**
	 * @brief Get the memory_resource where this buffer is stored. 
	 * 
	 * @return memory_resource& The resource where the buffer is stored.
	 */
	XMIPP4_CORE_API 
	hardware::memory_resource& get_memory_resource() const noexcept;

	/**
	 * @brief Acknowledge that this storage is being used in a device_queue.
	 * 
	 * Due to the asynchronous nature of the device_queue-s, the buffer may
	 * be needed after its destruction on the application code. By default,
	 * protections against this race condition only exist with the queue used
	 * to allocate this buffer (if any). When using this buffer in additional 
	 * queues, this method should be called to synchronize its destruction.
	 * 
	 * This method should only be used if the memory resource associated to 
	 * this buffer has a device associated to it.
	 * 
	 * @param queue The queue where this buffer in being used. It must have
	 * been created from the same device as the device targeted by the 
	 * memory_resource.
	 * @param exclusive If true, it disregards all previous queues where it was 
	 * being used and it synchronizes only with the new one. This is useful
	 * if the new queue is externally synchronized such that all other accesses 
	 * are guaranteed to have been concluded before they're completed at the 
	 * provided queue.
	 * 
	 * @throws invalid_operation_error when the buffer does not support
	 * queue execution.
	 * 
	 */
	XMIPP4_CORE_API 
	void record_queue(hardware::device_queue &queue, bool exclusive=false);

private:
	std::shared_ptr<hardware::buffer> m_buffer;

};

} // namespace multidimensional
} // namespace xmipp4
