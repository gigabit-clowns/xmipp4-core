// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Abstract class describing a command device_queue.
 * 
 */
class XMIPP4_CORE_API device_queue
{
public:
    device_queue() = default;
    device_queue(const device_queue &other) = default;
    device_queue(device_queue &&other) = default;
    virtual ~device_queue() = default;

    device_queue& operator=(const device_queue &other) = default;
    device_queue& operator=(device_queue &&other) = default;

    /**
     * @brief Wait until the device_queue is flushed.
     * 
     */
    virtual void wait_until_completed() const = 0;

    /**
     * @brief Check if the queue has completed processing.
     * 
     * @return true Queue has finished processing.
     * @return false Queue is busy processing.
     */
    virtual bool is_idle() const noexcept = 0;

}; 

} // namespace compute
} // namespace xmipp4
