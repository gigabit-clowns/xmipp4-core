// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/memory_allocator.hpp>

#include <memory>
#include <unordered_map>

namespace xmipp4
{
namespace hardware
{

class memory_resource;

/**
 * @brief Lazy cache that maps each @ref memory_resource to a corresponding
 * @ref memory_allocator.
 *
 * Allocators are created on first access and reused on subsequent accesses.
 * The pool owns the allocators for its entire lifetime.
 */
class memory_allocator_pool
{
public:
	memory_allocator_pool() = default;
	memory_allocator_pool(const memory_allocator_pool &other) = delete;
	memory_allocator_pool(memory_allocator_pool &&other) = default;
	~memory_allocator_pool() = default;

	memory_allocator_pool&
	operator=(const memory_allocator_pool &other) = delete;
	memory_allocator_pool&
	operator=(memory_allocator_pool &&other) = default;

	/**
	 * @brief Return the allocator associated with @p resource, creating
	 * it if it does not exist yet.
	 *
	 * On the first call for a given @p resource,
	 * @ref memory_resource::create_allocator is invoked and the result is
	 * cached. Subsequent calls for the same @p resource return the cached
	 * allocator without calling @ref memory_resource::create_allocator again.
	 *
	 * @param resource The memory resource whose allocator is requested.
	 * @return Shared pointer to the allocator for @p resource.
	 */
	const std::shared_ptr<memory_allocator>& touch(memory_resource &resource);

private:
	using map_type = std::unordered_map<
		const memory_resource*, 
		std::shared_ptr<memory_allocator>
	>;
	map_type m_allocators;
};

} // namespace hardware
} // namespace xmipp4

