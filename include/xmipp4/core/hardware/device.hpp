// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>
#include <vector>

#include <xmipp4/core/platform/dynamic_shared_object.h>

#include "device_properties.hpp"
#include "memory_resource_affinity.hpp"
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
	/**
	 * @brief Construct a device from its static properties.
	 *
	 * The properties are supplied by the @ref device_backend that creates
	 * the device (typically the same values it reports through
	 * @ref device_backend::get_device_properties) and are stored for the
	 * lifetime of the device, after which they remain immutable.
	 *
	 * @param[in] properties The static characteristics describing this
	 * device. Taken by value and moved into the device.
	 */
	explicit device(device_properties properties) noexcept;
	device(const device &other) = delete;
	device(device &&other) = delete;
	virtual ~device();

	device& operator=(const device &other) = delete;
	device& operator=(device &&other) = delete;

	/**
	 * @brief Retrieve the static properties of this device.
	 *
	 * Returns the @ref device_properties supplied at construction. The
	 * referenced object lives as long as this @c device and does not
	 * change after construction.
	 *
	 * @return A reference to this device's @ref device_properties.
	 *
	 * @see device_properties
	 */
	const device_properties& get_properties() const noexcept;

	/**
	 * @brief Retrieve a memory resource by its affinity.
	 *
	 * Returns the @c memory_resource that is best suited for the access pattern
	 * described by @p affinity. The returned reference is valid for the
	 * lifetime of this @c device.
	 *
	 * @note Depending on the backend, two distinct affinity values may resolve
	 * to the same underlying @c memory_resource (i.e. the references may
	 * alias).
	 *
	 * @param affinity The intended access pattern for which a resource is
	 * requested.
	 * @return A reference to the matching @c memory_resource.
	 *
	 * @see memory_resource_affinity
	 * @see memory_resource
	 */
	virtual
	const memory_resource&
	get_memory_resource(memory_resource_affinity affinity) const = 0;

	/**
	 * @brief Get the default command queue for this device.
	 *
	 * Returns a persistent command queue associated with this device. This 
	 * queue is created at device initialization and may be reused across the 
	 * lifetime of the device. Multiple calls to this method return the same 
	 * reference.
	 *
	 * @return A shared pointer to the default command queue. Never null.
	 *
	 * @see command_queue
	 * @see create_command_queue()
	 */
	virtual
	std::shared_ptr<command_queue> get_default_queue() const = 0;

	/**
	 * @brief Create a new command queue on this device.
	 *
	 * The returned queue is independent from any previously created queue on 
	 * the same device, including the default queue and may execute its commands
	 * concurrently with them. Ordering across queues may be established through
	 * @ref event objects.
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

private:
	device_properties m_properties;
};

} // namespace hardware
} // namespace xmipp4
