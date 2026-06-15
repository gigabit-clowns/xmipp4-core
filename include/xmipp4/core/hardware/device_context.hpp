// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include <xmipp4/core/platform/dynamic_shared_object.h>

#include "memory_resource_affinity.hpp"
#include "event_usage_flags.hpp"

namespace xmipp4
{
namespace hardware
{

class device;
class device_properties;
class command_queue;
class event;
class memory_resource;
class memory_allocator;

/**
 * @brief Facade bundling a @ref device with the per-device resources that the
 * framework manages on top of it.
 *
 * A @c device_context pairs a hardware @ref device with the memory allocators
 * built for it, exposing a single object that the rest of the framework can
 * use to allocate memory and create per-device objects (queues, events, ...)
 * without having to wire the allocators manually.
 *
 * The device-facing methods forward to the underlying @ref device, while the
 * allocator-facing methods operate on the owned allocators. The context is
 * move-only: copy construction and copy assignment are deleted.
 *
 * A context may also be @em empty. A default-constructed context, as well as a
 * context that has been moved from, holds no device and owns no allocators.
 * Such a context is only meant to act as a placeholder until a real context is
 * move-assigned into it. Every accessor and device-facing method requires a
 * non-empty context as a precondition; emptiness can be queried through
 * @ref is_empty. Besides destruction, move-assignment and @ref is_empty,
 * invoking any method on an empty context results in undefined behavior.
 */
class device_context
{
public:
	/**
	 * @brief Construct an empty context that holds no device.
	 *
	 * The resulting context is a placeholder meant to be replaced through
	 * move-assignment before use; see @ref is_empty. Invoking any other method
	 * on it results in undefined behavior.
	 */
	XMIPP4_CORE_API
	device_context() noexcept;

	/**
	 * @brief Construct a context for @p dev.
	 *
	 * Builds the memory allocators by querying @p dev for its memory resources
	 * and selecting an allocator for each affinity.
	 *
	 * @param dev Device to wrap. Must not be null.
	 *
	 * @throws std::invalid_argument if @p dev is null.
	 * @throws std::runtime_error if a required memory_resource_affinity has no
	 * suitable resource on @p dev.
	 */
	XMIPP4_CORE_API
	explicit device_context(std::shared_ptr<device> dev);
	device_context(const device_context &other) = delete;
	XMIPP4_CORE_API
	device_context(device_context &&other) noexcept;
	XMIPP4_CORE_API
	~device_context();

	device_context& operator=(const device_context &other) = delete;
	XMIPP4_CORE_API
	device_context& operator=(device_context &&other) noexcept;

	/**
	 * @brief Check whether this context is empty.
	 *
	 * A context is empty when it has been default-constructed or moved from,
	 * in which case it holds no device and owns no allocators.
	 *
	 * @return true if the context holds no device, false otherwise.
	 */
	XMIPP4_CORE_API
	bool is_empty() const noexcept;

	/**
	 * @brief Retrieve the wrapped device.
	 *
	 * @pre This context must not be empty.
	 * @return Shared pointer to the device.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<device>& get_device() const noexcept;

	/**
	 * @brief Retrieve a memory resource by its affinity.
	 *
	 * Forwards to @ref device::get_memory_resource.
	 *
	 * @param affinity The intended access pattern for which a resource is
	 * requested.
	 * @pre This context must not be empty.
	 * @return A reference to the matching @ref memory_resource.
	 */
	XMIPP4_CORE_API
	const memory_resource&
	get_memory_resource(memory_resource_affinity affinity) const;

	/**
	 * @brief Retrieve the static properties of the wrapped device.
	 *
	 * Forwards to @ref device::get_properties.
	 *
	 * @pre This context must not be empty.
	 * @return A reference to the device's @ref device_properties.
	 */
	XMIPP4_CORE_API
	const device_properties& get_properties() const noexcept;

	/**
	 * @brief Retrieve the allocator for the given memory affinity.
	 *
	 * @param affinity The desired memory_resource_affinity (host or device).
	 * @pre This context must not be empty.
	 * @return Shared pointer to the corresponding memory_allocator.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<memory_allocator>&
	get_allocator(memory_resource_affinity affinity) const noexcept;

	/**
	 * @brief Replace the allocator for the given memory affinity.
	 *
	 * @param affinity The memory_resource_affinity slot to update.
	 * @param allocator New allocator to install; may be nullptr to clear the
	 * slot.
	 * @pre This context must not be empty.
	 * @return The allocator that was previously installed in that slot, or
	 * nullptr if the slot was empty.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<memory_allocator>
	set_allocator(
		memory_resource_affinity affinity,
		std::shared_ptr<memory_allocator> allocator
	) noexcept;

	/**
	 * @brief Get the default command queue for the wrapped device.
	 *
	 * Forwards to @ref device::get_default_queue.
	 *
	 * @pre This context must not be empty.
	 * @return A shared pointer to the default command queue. Never null.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<command_queue> get_default_queue() const;

	/**
	 * @brief Create a new command queue on the wrapped device.
	 *
	 * Forwards to @ref device::create_command_queue.
	 *
	 * @pre This context must not be empty.
	 * @return Newly created queue. Never null.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<command_queue> create_command_queue() const;

	/**
	 * @brief Create a synchronization primitive on the wrapped device.
	 *
	 * Forwards to @ref device::create_event.
	 *
	 * @param usage Capabilities that the returned event must support.
	 * @pre This context must not be empty.
	 * @return Newly created event. Never null.
	 */
	XMIPP4_CORE_API
	std::shared_ptr<event> create_event(event_usage_flags usage) const;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;
};

} // namespace hardware
} // namespace xmipp4
