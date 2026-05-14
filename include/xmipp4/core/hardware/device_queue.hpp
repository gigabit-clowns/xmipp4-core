// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief Abstract command queue belonging to a @ref device.
 *
 * A @c device_queue represents an in-order stream of commands submitted
 * to a device (kernel launches, memory transfers, event signals, ...).
 * Commands submitted to the same queue execute in submission order;
 * commands in different queues may execute concurrently and need
 * @ref device_event objects to express ordering between them.
 */
class XMIPP4_CORE_API device_queue
{
public:
	device_queue() noexcept;
	device_queue(const device_queue &other) = delete;
	device_queue(device_queue &&other) = delete;
	virtual ~device_queue();

	device_queue& operator=(const device_queue &other) = delete;
	device_queue& operator=(device_queue &&other) = delete;

	/**
	 * @brief Block the calling thread until the queue is idle.
	 *
	 * Suspends the host thread until every command submitted to this queue 
	 * before the call has completed execution. Commands submitted after the 
	 * call returns are not awaited, so the queue may again become non-idle as
	 * soon as new work is submitted.
	 *
	 * Equivalent in spirit to @ref device_event::wait on an event signaled at 
	 * the most recent submission point, but cheaper when no event is required 
	 * afterwards.
	 */
	virtual void wait_until_completed() const = 0;

	/**
	 * @brief Non-blocking query of whether the queue is idle.
	 *
	 * Reports whether every command submitted to this queue before the call has
	 * completed, without blocking the calling thread. A queue with no submitted
	 * work is reported as idle.
	 *
	 * @return @c true if the queue has finished all previously submitted work, 
	 * @c false if any of it is still pending.
	 */
	virtual bool is_idle() const = 0;
};

} // namespace hardware
} // namespace xmipp4
