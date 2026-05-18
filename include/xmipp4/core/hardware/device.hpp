// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include <vector>

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
 * A @c device is the entry point for interacting with a backend's
 * compute resource: it advertises the memory resources reachable from
 * the device and acts as a factory for the per-device objects used by
 * the rest of the framework (queues, synchronization primitives, ...).
 */
class XMIPP4_CORE_API device
{
public:
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
	 * Ordering across queues may be established through @ref event 
	 * objects.
	 *
	 * @return Shared ownership of the newly created queue. Never null.
	 */
	virtual std::shared_ptr<command_queue> create_command_queue() const = 0;

	/**
	 * @brief Create a synchronization primitive for this device.
	 *
	 * The returned event supports at least the operations requested in
	 * @p usage; the backend may pick the cheapest underlying primitive that 
	 * satisfies them, so the actually supported set (queried via
	 * @ref event::get_supported_usage) may be a superset of @p usage. 
	 * The event is initially in the signaled state.
	 *
	 * @param usage Capabilities that the returned event must support.
	 * @return Shared ownership of the newly created event. Never null.
	 *
	 * @see event
	 * @see event_usage_flags
	 */
	virtual
	std::shared_ptr<event> create_event(event_usage_flags usage) const = 0;
};

} // namespace hardware
} // namespace xmipp4
