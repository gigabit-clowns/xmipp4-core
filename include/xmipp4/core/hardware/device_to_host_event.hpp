// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class device_queue;



/**
 * @brief Abstract class describing a device to host synchronization primitive.
 * 
 * Instances of this inteface may be used to query or ensure
 * that a certain point in the execution pipeline has
 * been reached.
 * 
 * @note When constructing a new event, it is constructed in the signaled
 * state, such that waiting on it does not block execution. Similarly,
 * is_signaled returns true.
 * 
 */
class XMIPP4_CORE_API device_to_host_event
{
public:
    device_to_host_event() = default;
    device_to_host_event(const device_to_host_event &other) = default;
    device_to_host_event(device_to_host_event &&other) = default;
    virtual ~device_to_host_event() = default;

    device_to_host_event& operator=(const device_to_host_event &other) = default;
    device_to_host_event& operator=(device_to_host_event &&other) = default;

    /**
     * @brief Signal the event when the current point in the execution 
     * queue is reached.
     * 
     * @param queue The queue where the event is recorded.
     * 
     */
    virtual void signal(device_queue &queue) = 0;

    /**
     * @brief Check if the last recorded event is signaled by the queue.
     * 
     * @return true The event was signaled by the queue.
     * @return false The event has not been signaled by the queue.
     * 
     */
    virtual bool is_signaled() const = 0;

    /**
     * @brief Wait until the last recorded event is signaled by the queue.
     * 
     */
    virtual void wait() const = 0;

}; 

} // namespace hardware
} // namespace xmipp4
