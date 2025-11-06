// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device_queue;

/**
 * @brief Abstract class to inform a memory allocator about the status of
 * an allocation.
 * 
 */
class XMIPP4_CORE_API memory_allocation_tracker
{
public:
    memory_allocation_tracker() = default;
    memory_allocation_tracker(const memory_allocation_tracker &other) = default;
    memory_allocation_tracker(memory_allocation_tracker &&other) = default;
    virtual ~memory_allocation_tracker() = default;

    memory_allocation_tracker& 
    operator=(const memory_allocation_tracker &other) = default;
    memory_allocation_tracker& 
    operator=(memory_allocation_tracker &&other) = default;

    /**
     * @brief Signal that the allocation is being used on extra queues.
     * 
     * @param queue Queue where the allocation is being used.
     * @param exclusive Whether the new queue is explicitly synchronized
     * with the previous usage-s.
     */
    virtual void record_queue(device_queue &queue, bool exclusive);

}; 

} // namespace hardware
} // namespace xmipp4
