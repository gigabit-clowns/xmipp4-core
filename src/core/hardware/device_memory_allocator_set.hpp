// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_allocator.hpp>
#include <xmipp4/core/hardware/memory_resource_affinity.hpp>

#include <memory>
#include <array>

namespace xmipp4
{
namespace hardware
{

class device;
class memory_resource;

/**
 * @brief Owns one memory_allocator per memory_resource_affinity value.
 *
 * Aggregates the allocators required to serve allocation requests on a
 * single device. Each memory_resource_affinity slot holds an independent
 * shared_ptr<memory_allocator> that can be queried or replaced individually.
 *
 * The set is move-only: copy construction and copy assignment are deleted.
 */
class device_memory_allocator_set
{
public:
	device_memory_allocator_set() = default;

	/**
	 * @brief Construct by querying @p dev for its memory resources and 
	 * selecting the best allocator for each affinity.
	 *
	 * @param dev Device whose memory resources are enumerated.
	 *
	 * @throws std::runtime_error if a required memory_resource_affinity
	 * has no suitable resource on @p dev.
	 */
	explicit device_memory_allocator_set(const device &dev);
	device_memory_allocator_set(
		const device_memory_allocator_set &other) = delete;
	device_memory_allocator_set(
		device_memory_allocator_set &&other) = default;
	~device_memory_allocator_set() = default;

	device_memory_allocator_set&
	operator=(const device_memory_allocator_set &other) = delete;
	device_memory_allocator_set&
	operator=(device_memory_allocator_set &&other) = default;

	/**
	 * @brief Replace the allocator for the given memory affinity.
	 *
	 * @param affinity The memory_resource_affinity slot to update.
	 * @param allocator New allocator to install; may be nullptr to clear
	 * the slot.
	 * @return The allocator that was previously installed in that slot,
	 * or nullptr if the slot was empty.
	 */
	std::shared_ptr<memory_allocator>
	set_allocator(
		memory_resource_affinity affinity,
		std::shared_ptr<memory_allocator> allocator
	) noexcept;

	/**
	 * @brief Retrieve the allocator for the given memory affinity.
	 *
	 * @param affinity The desired memory_resource_affinity (host or device).
	 * @return Shared pointer to the corresponding memory_allocator.
	 */
	const std::shared_ptr<memory_allocator>&
	get_allocator(memory_resource_affinity affinity) const noexcept;

private:
	using allocator_array_type = std::array<
		std::shared_ptr<memory_allocator>,
		static_cast<std::size_t>(memory_resource_affinity::count)
	>;
	allocator_array_type m_allocators;
};

} // namespace hardware
} // namespace xmipp4
