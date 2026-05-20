// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/span.hpp>

#include <memory>

namespace xmipp4
{
namespace hardware
{

class buffer;
class command;
class event;

/**
 * @brief Abstract command queue belonging to a @ref device.
 *
 * A @c command_queue represents an in-order stream of commands submitted
 * to a device (kernel launches, memory transfers...). Commands submitted to the
 * same queue execute in submission order; commands in different queues may 
 * execute concurrently.
 */
class XMIPP4_CORE_API command_queue
{
public:
	command_queue() noexcept;
	command_queue(const command_queue &other) = delete;
	command_queue(command_queue &&other) = delete;
	virtual ~command_queue();

	command_queue& operator=(const command_queue &other) = delete;
	command_queue& operator=(command_queue &&other) = delete;

	/**
	 * @brief Submit a command for execution on this queue.
	 *
	 * Schedules @p command to run at the current point of this queue's
	 * execution timeline, operating on the supplied operands. The call
	 * returns as soon as the work has been recorded; execution proceeds
	 * asynchronously with respect to the host thread, ordered with respect
	 * to other commands previously submitted to the same queue.
	 *
	 * @p output_operands correspond to the read-write descriptors passed at
	 * @p command construction time, in the same order; @p input_operands
	 * correspond to the read-only descriptors. None of the pointers in
	 * either span may be null. @p command must have been constructed for
	 * the same device as this queue.
	 *
	 * @param command         The command to execute.
	 * @param output_operands Operands the command may write to.
	 * @param input_operands  Operands the command may only read from.
	 */
	virtual void submit(
		const command &command,
		span<const std::shared_ptr<buffer>> output_operands,
		span<const std::shared_ptr<const buffer>> input_operands
	) = 0;

	/**
	 * @brief Record a signal on this queue for the given event.
	 *
	 * Schedules @p event to transition to the signaled state once this queue
	 * reaches the current point in its execution timeline. Any previously
	 * recorded signal point on @p event is superseded.
	 *
	 * Always supported on every event, regardless of
	 * @ref event::get_supported_usage. 
	 *
	 * @param event The event whose signal point is recorded at the current
	 * point of this queue's timeline.
	 * 
	 * @note @p event must belong to the same device as this queue.
	 */
	virtual void signal(event &event) = 0;

	/**
	 * @brief Defer subsequent work on this queue until an event is signaled.
	 *
	 * Schedules this queue so that commands submitted after the call are
	 * deferred until the most recently recorded signal point on @p event has
	 * been reached. The call itself does not block the host thread.
	 *
	 * Requires @ref event_usage_flag_bits::device_wait on @p event. This
	 * queue may belong to a different device than the one that created the
	 * event only if @ref event_usage_flag_bits::cross_device_wait is also
	 * supported by @p event.
	 *
	 * @param event The event whose signal point this queue will wait for
	 * before executing any further submitted command.
	 */
	virtual void wait(const event &event) = 0;

	/**
	 * @brief Block the calling thread until the queue is idle.
	 *
	 * Suspends the host thread until every command submitted to this queue
	 * before the call has completed execution. Commands submitted after the
	 * call returns are not awaited, so the queue may again become non-idle as
	 * soon as new work is submitted.
	 *
	 * Equivalent in spirit to @ref event::wait on an event signaled at
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
