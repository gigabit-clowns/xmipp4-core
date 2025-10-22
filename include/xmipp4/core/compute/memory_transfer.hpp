// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include "../span.hpp"
#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
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
     * @param source The buffer from which data is copied. It's memory_resource
     * must equal to the memory resource returned by get_source_memory_resource.
     * @param destination The buffer from which data is copied. It's 
     * memory_resource must equal to the memory resource returned by 
     * get_destination_memory_resource.
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

/**
 * @brief Get a memory_transfer that can copy data to and from host memory.
 * 
 * The returned object is able to copy data between any memory resource
 * as long as they are host accessible, i.e, get_host_ptr() does not return
 * nullptr.
 * 
 * @return const memory_transfer& The memory transfer object able to handle
 * host memory transfers.
 */
const memory_transfer& get_host_memory_transfer() noexcept;

} // namespace compute
} // namespace xmipp4
