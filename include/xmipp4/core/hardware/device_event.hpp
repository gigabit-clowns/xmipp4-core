// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/hardware/device_timeline_clock.hpp>
#include <xmipp4/core/hardware/device_event_usage_flags.hpp>

namespace xmipp4
{
namespace hardware
{

class device_queue;

/**
 * @brief Abstract synchronization primitive between queues and the host.
 *
 * A @c device_event marks a point in a device queue's execution timeline that
 * can later be waited on or queried, either by another queue (possibly on a 
 * different device) or by the host thread. 
 *
 * A newly constructed event is in the signaled state. Any wait or query issued
 * before the first call to @ref signal completes immediately and reports the 
 * event as signaled.
 * 
 * @ref signal may be invoked multiple times over the lifetime of the event. 
 * Each call defines a new target point in the corresponding queue's timeline 
 * and supersedes any previous one; subsequent waits and queries refer to the 
 * most recently recorded point. There is no need for the user to reset the 
 * event between uses; backends that require an explicit reset handle it 
 internally. The event can only be signaled from the device used to create it, 
 * even if @ref device_event_usage_flag_bits::cross_device_wait is set.
 * 
 * The supported set of operations is determined at creation time by the 
 * @ref device_event_usage_flags passed to the factory, and can be queried 
 * via @ref get_supported_usage. Backends are free to pick the cheapest 
 * underlying primitive that satisfies the requested capabilities; the actually
 * supported set may be a superset of the requested one. Invoking an operation 
 * whose capability bit is not advertised by @ref get_supported_usage is a 
 * precondition violation and yields undefined behavior.
 */
class XMIPP4_CORE_API device_event
{
public:
	device_event() noexcept;
	device_event(const device_event &other) = delete;
	device_event(device_event &&other) = delete;
	virtual ~device_event();

	device_event& operator=(const device_event &other) = delete;
	device_event& operator=(device_event &&other) = delete;

	/**
	 * @brief Get the set of operations supported by this event.
	 */
	virtual device_event_usage_flags get_supported_usage() const noexcept = 0;

	/**
	 * @brief Record a signal on a device queue.
	 *
	 * Schedules the event to transition to the signaled state once the queue 
	 * reaches the current point in its execution timeline. Any previously 
	 * recorded signal point on this event is superseded.
	 *
	 * Always supported, regardless of @ref get_supported_usage.
	 *
	 * @param queue The queue on which the signal is recorded. Must belong to
	 * the same device used for creating this event.
	 */
	virtual void signal(device_queue &queue) = 0;

	/**
	 * @brief Make a device queue wait until the event is signaled.
	 *
	 * Schedules @p queue so that subsequent commands submitted to it are 
	 * deferred until the most recently recorded signal point on this event has
	 * been reached. The call itself does not block the host thread.
	 *
	 * Requires @ref device_event_usage_flag_bits::device_wait. The @p queue may
	 * belong to a different device than the one that recorded the signal only 
	 * if @ref device_event_usage_flag_bits::cross_device_wait is also 
	 * supported.
	 *
	 * @param queue The queue that will defer its work until the event
	 * is signaled.
	 */
	virtual void wait(device_queue &queue) const = 0;

	/**
	 * @brief Block the calling thread until the event is signaled.
	 *
	 * Suspends the host thread until the most recently recorded signal point on
	 * this event has been reached. Returns immediately if no signal has been
	 * recorded yet, since the initial state of the event is signaled.
	 *
	 * Requires @ref device_event_usage_flag_bits::host_wait.
	 */
	virtual void wait() const = 0;

	/**
	 * @brief Non-blocking query of the event's state.
	 *
	 * Reports whether the most recently recorded signal point has been reached
	 * by the queue, without blocking the calling thread. An event with no
	 * recorded signal is reported as signaled.
	 *
	 * Requires @ref device_event_usage_flag_bits::host_query.
	 *
	 * @return @c true if the event has been signaled, @c false otherwise.
	 */
	virtual bool is_signaled() const = 0;

	/**
	 * @brief Get the time at which the most recently recorded signal point was
	 * reached.
	 *
	 * Returns a time point on @ref device_timeline_clock corresponding to the
	 * moment the queue passed the most recently recorded signal point on this
	 * event. The value lies on the device's local timeline; see
	 * @ref device_timeline_clock for comparison rules.
	 *
	 * The call blocks the calling host thread until that signal point has been
	 * reached, in the same way @ref wait does. Callers must invoke @ref signal
	 * at least once before querying the timestamp; the behavior of this method
	 * on an event that has never been signaled is unspecified.
	 *
	 * @return Time point of the last reached signal point.
	 */
	virtual device_timeline_clock::time_point get_timestamp() const = 0;
};

} // namespace hardware
} // namespace xmipp4
