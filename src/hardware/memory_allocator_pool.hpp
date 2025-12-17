// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/memory_allocator.hpp>

#include <array>
#include <unordered_map>
#include <memory>

namespace xmipp4
{
namespace hardware
{

class device;
class memory_resource;
class memory_allocator_manager;

/**
 * @brief Collection of memory allocators related to a particular device.
 * 
 * This class organizes memory allocators efficiently by overlapping them when 
 * memory_resources alias one another (like in unified architectures).
 * 
 */
class memory_allocator_pool
{
public:
	memory_allocator_pool(
		device& dev, 
		const memory_allocator_manager &allocator_manager
	);
	memory_allocator_pool(const memory_allocator_pool& other) = delete;
	memory_allocator_pool(memory_allocator_pool&& other) = delete;
	~memory_allocator_pool() = default;

	memory_allocator_pool&
	operator=(const memory_allocator_pool& other) = delete;
	memory_allocator_pool&
	operator=(memory_allocator_pool&& other) = delete;

	/**
	 * @brief Get the memory allocator for the requested affinity.
	 * 
	 * @param affinity Intended usage for the buffers created with the returned 
	 * allocator.
	 * @return memory_allocator& The memory allocator.
	 */
	memory_allocator& 
	get_memory_allocator(memory_resource_affinity affinity) const;

private:
	std::unordered_map<
		memory_resource*, 
		std::shared_ptr<memory_allocator>
	> m_allocators;
	std::array<
		memory_allocator*, 
		static_cast<std::size_t>(memory_resource_affinity::count)
	> m_allocators_by_placement;
};

} // namespace hardware
} // namespace xmipp4
