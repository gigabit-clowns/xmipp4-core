// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include <vector>
#include <chrono>

#include <xmipp4/core/platform/dynamic_shared_object.h>

#include "event_usage_flags.hpp"

namespace xmipp4 
{
namespace hardware
{

class command_queue;
class event;
class memory_resource;

/**
 * @brief Abstract representation of a hardware compute device.
 *
 * A @c device is the entry point for interacting with a backend's compute
 * resource: it advertises the memory resources reachable from the device and
 * acts as a factory for the per-device objects used by the rest of the
 * framework (queues, synchronization primitives, ...).
 */
class XMIPP4_CORE_API device
{
public:
	using duration_type = std::chrono::nanoseconds;

	device() noexcept;
	device(const device &other) = delete;
	device(device &&other) = delete;
	virtual ~device();

	device& operator=(const device &other) = delete;
	device& operator=(device &&other) = delete;

	/**
	 * @brief Get the memory resources reachable from this device.
	 *
	 * Populates @p resources with pointers to every memory resource that this 
	 * device can allocate from or interact with. The set typically includes the
	 * device's local memory and may include host-side resources usable for 
	 * transfers (e.g. pinned host memory). The pointed-to resources are owned 
	 * by the backend and remain valid for as long as the device does.
	 *
	 * @param[out] resources Output container, cleared and filled with the 
	 * memory resources known to this device. None of the stored pointers are 
	 * null.
	 */
	virtual
	void get_memory_resources(
		std::vector<const memory_resource*> &resources
	) const = 0;

	/**
	 * @brief Create a new command queue on this device.
	 *
	 * The returned queue is independent from any previously created queue on 
	 * the same device, and may execute its commands concurrently with them. 
	 * Ordering across queues may be established through @ref event objects.
	 *
	 * @return Newly created queue. Never null.
	 */
	virtual std::shared_ptr<command_queue> create_command_queue() const = 0;

	/**
	 * @brief Create a synchronization primitive for this device.
	 *
	 * The returned event supports at least the operations requested in
	 * @p usage; the backend may pick the cheapest underlying primitive that 
	 * satisfies them, so the actually supported set (queried via
	 * @ref event::get_supported_usage) may be a superset of @p usage. 
	 * The event is initially in the signaled state (waiting on it returns 
	 * immediately).
	 *
	 * @param usage Capabilities that the returned event must support.
	 * @return Newly created event. Never null.
	 *
	 * @see event
	 * @see event_usage_flags
	 */
	virtual
	std::shared_ptr<event> create_event(event_usage_flags usage) const = 0;

	/**
	 * @brief Measure the time elapsed between two event signal points.
	 *
	 * Returns the duration from the most recently recorded signal point on
	 * @p start to the most recently recorded signal point on @p stop, as
	 * observed on this device's execution timeline. The result is signed: if
	 * @p stop was reached before @p start the returned duration is negative.
	 *
	 * The call blocks the calling host thread until both signal points have
	 * been reached, in the same way @ref event::wait does.
	 *
	 * Both events must belong to this device and must have been created with
	 * @ref event_usage_flag_bits::timestamp. Both must have been signaled at
	 * least once before this method is invoked; the behavior on an event that
	 * has never been signaled is unspecified. Passing an event that belongs
	 * to a different device is a precondition violation.
	 *
	 * The resolution of the returned duration is backend-dependent and is
	 * typically coarser than the nanosecond representation suggests.
	 *
	 * @param start Event marking the beginning of the measured interval.
	 * @param stop  Event marking the end of the measured interval.
	 * @return Signed duration from @p start to @p stop on this device's
	 * timeline.
	 *
	 * @see event
	 * @see event_usage_flag_bits::timestamp
	 */
	virtual
	duration_type elapsed_time(const event& start, const event& stop) const = 0;
};

} // namespace hardware
} // namespace xmipp4
