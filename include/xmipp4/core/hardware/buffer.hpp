// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "buffer_sentinel.hpp"
#include "memory_resource.hpp"
#include "../platform/dynamic_shared_object.h"

#include <utility>
#include <memory>
#include <cstddef>

namespace xmipp4 
{
namespace hardware
{

class device_queue;



/**
 * @brief Represents an untyped memory allocation at a given memory
 * resource.
 * 
 */
class buffer
{
public:
    XMIPP4_CORE_API buffer(
        void *host_pointer,
        std::size_t size,
        std::reference_wrapper<memory_resource> resource,
        std::unique_ptr<buffer_sentinel> sentinel
    );
    buffer(const buffer &other) = delete;
    XMIPP4_CORE_API buffer(buffer &&other) noexcept;
    XMIPP4_CORE_API virtual ~buffer();

    buffer& operator=(const buffer &other) = delete;
    XMIPP4_CORE_API buffer& operator=(buffer &&other) noexcept;

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
    XMIPP4_CORE_API 
    void* get_host_ptr() noexcept;

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
    XMIPP4_CORE_API 
    const void* get_host_ptr() const noexcept;

    /**
     * @brief Get the size in bytes for this buffer.
     * 
     * @return std::size_t Size in bytes.
     */
    XMIPP4_CORE_API 
    std::size_t get_size() const noexcept;

    /**
     * @brief Get the memory_resource where this buffer is stored. 
     * 
     * @return memory_resource& The resource where the buffer is stored.
     */
    XMIPP4_CORE_API 
    memory_resource& get_memory_resource() const noexcept;

    /**
     * @brief Get the sentinel for this buffer.
     * 
     * @return buffer_sentinel* Pointer to the sentinel.
     */
    XMIPP4_CORE_API
    buffer_sentinel* get_sentinel() noexcept;

    /**
     * @brief Get the sentinel for this buffer.
     * 
     * @return buffer_sentinel* Pointer to the sentinel.
     */
    XMIPP4_CORE_API
    const buffer_sentinel* get_sentinel() const noexcept;

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
     * 
     * @throws invalid_operation_error when the buffer does not support
     * queue execution.
     * 
     */
    XMIPP4_CORE_API 
    void record_queue(device_queue &queue, bool exclusive=false);

private:
    void *m_host_pointer;
    std::size_t m_size;
    std::reference_wrapper<memory_resource> m_memory_resource;
    std::unique_ptr<buffer_sentinel> m_sentinel;

};

} // namespace hardware
} // namespace xmipp4
