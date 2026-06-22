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
class program;
class event;

/**
 * @brief Abstract command queue belonging to a @ref device.
 *
 * A @c command_queue represents an in-order stream of programs submitted
 * to a device (kernel launches, memory transfers...). Programs submitted to the
 * same queue execute in submission order; programs in different queues may
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
	 * @brief Submit a program for execution on this queue.
	 *
	 * Schedules @p prog to run at the current point of this queue's
	 * execution timeline, operating on the supplied operands. The call
	 * returns as soon as the work has been recorded; execution proceeds
	 * asynchronously with respect to the host thread, ordered with respect
	 * to other programs previously submitted to the same queue.
	 *
	 * @p output_operands correspond to the read-write descriptors passed at
	 * @p prog construction time, in the same order; @p input_operands
	 * correspond to the read-only descriptors. None of the pointers in
	 * either span may be null. @p prog must have been constructed for
	 * the same device as this queue.
	 *
	 * @p scratch supplies transient workspaces that the program may use
	 * during execution. The requirements for scratch buffers are queried
	 * via @ref program::get_scratch_requirements, which populates a vector
	 * with the needed scratch buffer specifications. If the vector is empty,
	 * the program requires no scratch buffers and @p scratch may be empty.
	 * Otherwise, @p scratch must contain one buffer for each requirement in
	 * the order returned. Each buffer must have a size >= the corresponding
	 * requirement's size and alignment >= the requirement's alignment.
	 * The buffers must remain valid until the program completes; the queue
	 * holds references for that duration. Contents are undefined on entry and
	 * on completion, so callers must not store persistent data in them and
	 * may reuse the same buffers for multiple programs as long as their
	 * executions do not overlap. All scratch buffers must be allocated on,
	 * and accessible to, the same device as this queue.
	 *
	 * @param prog The program to execute.
	 * @param output_operands Operands the program may write to.
	 * @param input_operands  Operands the program may only read from.
	 * @param scratch Scratch buffers required by the program. The span size
	 * must match the number of requirements returned by
	 * @ref program::get_scratch_requirements.
	 */
	virtual void submit(
		const program &prog,
		span<const std::shared_ptr<buffer>> output_operands,
		span<const std::shared_ptr<const buffer>> input_operands,
		span<const std::shared_ptr<buffer>> scratch
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
	 * @throws invalid_operation_error if 
	 * @ref event_usage_flag_bits::device_wait is not present or this queue
	 * does not belong to the queue used for create @p and 
	 * @ref event_usage_flag_bits::coss_device_wait is not set.
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
