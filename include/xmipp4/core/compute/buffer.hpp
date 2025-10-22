// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <cstddef>
#include <typeinfo>

namespace xmipp4 
{
namespace compute
{

class device_queue;
class memory_resource;



/**
 * @brief Represents an untyped memory allocation at a given memory
 * resource.
 * 
 */
class XMIPP4_CORE_API buffer
{
public:
    buffer() = default;
    buffer(const buffer &other) = default;
    buffer(buffer &&other) = default;
    virtual ~buffer() = default;

    buffer& operator=(const buffer &other) = default;
    buffer& operator=(buffer &&other) = default;

    /**
     * @brief Get a host accessible pointer to the data.
     * 
     * This method only returns a pointer if the data is accessible by the 
     * host, i.e., if the kind of the underlying memory_resource is one of:
     * device_mapped, host_staging, unified or managed.
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
     * device_mapped, host_staging, unified or managed.
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
    virtual memory_resource& get_memory_resource() const noexcept = 0;

    /**
     * @brief Acknowledge that this buffer is being used in a device_queue.
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
     */
    virtual void record_queue(device_queue &queue, bool exclusive=false) = 0;

};

} // namespace compute
} // namespace xmipp4
