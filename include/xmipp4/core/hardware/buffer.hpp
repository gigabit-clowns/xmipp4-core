// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>

#include <utility>
#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace hardware
{

class memory_resource;
class memory_allocator;

/**
 * @brief Represents an untyped memory allocation at a given memory
 * resource.
 */
class XMIPP4_CORE_API buffer
{
public:
	buffer() noexcept;
	buffer(const buffer &other) = delete;
	buffer(buffer &&other) noexcept = delete;
	virtual ~buffer();

	buffer& operator=(const buffer &other) = delete;
	buffer& operator=(buffer &&other) noexcept = delete;

	/**
	 * @brief Get a host accessible pointer to the data.
	 * 
	 * This method only returns a pointer if the data is accessible by the 
	 * host, i.e., if the kind of the underlying memory_resource is one of:
	 * device_mapped, host_staging, host, unified or managed.
	 * 
	 * @return void* Pointer to the data. nullptr if the buffer is not
	 * host accessible.
	 */
	virtual void* get_host_ptr() noexcept = 0;

	/**
	 * @brief Get a host accessible pointer to the data.
	 * 
	 * This method only returns a pointer if the data is accessible by the 
	 * host, i.e., if the kind of the underlying memory_resource is one of:
	 * device_mapped, host_staging, host, unified or managed.
	 * 
	 * @return const void* Pointer to the data. nullptr if the buffer is not
	 * host accessible.
	 */
	virtual const void* get_host_ptr() const noexcept = 0;

	/**
	 * @brief Get the size in bytes for this buffer.
	 * 
	 * @return std::size_t Size in bytes.
	 */
	virtual std::size_t get_size() const noexcept = 0;

	/**
	 * @brief Get the memory_resource where this buffer is stored. 
	 * 
	 * @return memory_resource& The resource where the buffer is stored.
	 */
	virtual const memory_resource& get_memory_resource() const noexcept = 0;

	/**
	 * @brief Get the memory allocator used for allocating this buffer.
	 * 
	 * @return memory_allocator& The memory allocator where this buffer was
	 * allocated.
	 */
	virtual memory_allocator& get_memory_allocator() const noexcept = 0;
};

} // namespace hardware
} // namespace xmipp4
