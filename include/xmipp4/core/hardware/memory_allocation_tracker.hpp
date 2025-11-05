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
 * @brief Abstract class to inform a memory allocation about the status of
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

    // TODO
    virtual void record_queue(device_queue &queue, bool exclusive);

}; 

} // namespace hardware
} // namespace xmipp4
