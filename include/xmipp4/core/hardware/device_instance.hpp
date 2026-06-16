// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "device_properties.hpp"
#include "memory_resource_affinity.hpp"
#include "../platform/dynamic_shared_object.h"

#include <memory>

namespace xmipp4
{
namespace hardware
{

class device;
class memory_allocator;
class command_queue;

/**
 * @brief Immutable aggregate of device handle and related objects.
 *
 * A @c device_instance bundles a @ref device handle with the static
 * @ref device_properties describing it and a set of @ref memory_allocator
 * selected for each @ref memory_resource_affinity.
 *
 * Instances are neither copyable nor movable.
 */
class device_instance
{
public:
	/**
	 * @brief Construct an instance wrapping @p dev.
	 *
	 * Stores @p dev and @p properties and builds the allocator for each
	 * @ref memory_resource_affinity by querying @p dev for the matching
	 * memory resource.
	 *
	 * @param dev Device to wrap. Must not be null.
	 * @param properties Static properties describing @p dev. Taken by value
	 * and moved into the instance.
	 *
	 * @throws std::invalid_argument if @p dev is null.
	 * @throws std::runtime_error if a required @ref memory_resource_affinity
	 * has no suitable resource on @p dev.
	 */
	XMIPP4_CORE_API
	device_instance(
		std::shared_ptr<device> dev,
		device_properties properties
	);
	device_instance(const device_instance &other) = delete;
	device_instance(device_instance &&other) = delete;
	XMIPP4_CORE_API
	~device_instance();

	device_instance& operator=(const device_instance &other) = delete;
	device_instance& operator=(device_instance &&other) = delete;

	/**
	 * @brief Retrieve the wrapped device handle.
	 *
	 * @return A reference to the non-null device handle.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<device>& get_device() const noexcept;

	/**
	 * @brief Retrieve the static properties of the wrapped device.
	 *
	 * @return A reference to the device's @ref device_properties.
	 */
	XMIPP4_CORE_API
	const device_properties& get_properties() const noexcept;

	/**
	 * @brief Retrieve the allocator for the given affinity.
	 *
	 * The returned allocator is the one selected at construction for
	 * @p affinity and lives as long as this @c device_instance.
	 *
	 * @param affinity The desired memory_resource_affinity (host or device).
	 * @return A reference to the non-null allocator for @p affinity.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<memory_allocator>&
	get_allocator(memory_resource_affinity affinity) const noexcept;

	/**
	 * @brief Retrieve the default command queue of this instance.
	 *
	 * @return A reference to the non-null default command queue.
	 *
	 * @see command_queue
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<command_queue>& get_default_queue() const noexcept;

private:
	class implementation;
	std::unique_ptr<implementation> m_implementation;
};

} // namespace hardware
} // namespace xmipp4
