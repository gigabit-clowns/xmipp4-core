// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace hardware
{

class device_queue;

/**
 * @brief Abstract class describing a device event.
 * 
 * Instances of this inteface may be used to synchronize the execution
 * of commands across various queues of the same device.
 * 
 * @note When constructing a new event, it is constructed in the signaled
 * state, such that waiting on it does not block execution.
 * 
 */
class XMIPP4_CORE_API device_event
{
public:
	device_event() = default;
	device_event(const device_event &other) = default;
	device_event(device_event &&other) = default;
	virtual ~device_event() = default;

	device_event& operator=(const device_event &other) = default;
	device_event& operator=(device_event &&other) = default;

	/**
	 * @brief Signal the event when the current point in the execution 
	 * queue is reached.
	 * 
	 * @param queue The queue where the event is recorded.
	 * 
	 */
	virtual void signal(device_queue &queue) = 0;

	/**
	 * @brief Wait until the last recorded event is signaled by the queue.
	 * 
	 * @param queue The queue to be blocked until the event is signaled.
	 * 
	 */
	virtual void wait(device_queue &queue) const = 0;
}; 

} // namespace hardware
} // namespace xmipp4
