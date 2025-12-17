// SPDX-License-Identifier: GPL-3.0-only

#include "memory_allocator_pool.hpp"

#include <xmipp4/core/hardware/memory_allocator_manager.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4
{
namespace hardware
{

memory_allocator_pool::memory_allocator_pool(
	device& dev, 
	const memory_allocator_manager &allocator_manager
)
{
	for (std::size_t i = 0; i < m_allocators_by_placement.size(); ++i)
	{
		const auto affinity = static_cast<memory_resource_affinity>(i);
		auto &resource = dev.get_memory_resource(affinity);

		auto ite = m_allocators.find(&resource);
		if (ite == m_allocators.end())
		{
			std::tie(ite, std::ignore) = m_allocators.emplace(
				&resource, 
				allocator_manager.create_memory_allocator(resource)
			);
		}

		m_allocators_by_placement[i] = ite->second.get();
	}
}

memory_allocator& memory_allocator_pool::get_memory_allocator(
	memory_resource_affinity affinity
) const
{
	const auto index = static_cast<std::size_t>(affinity);
	auto *allocator = m_allocators_by_placement.at(index);
	XMIPP4_ASSERT(allocator);
	return *allocator;
}

} // namespace hardware
} // namespace xmipp4
