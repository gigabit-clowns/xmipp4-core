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
	m_allocators_by_placement = {};

	for (std::size_t i = 0; i < m_allocators_by_placement.size(); ++i)
	{
		const auto placement = static_cast<target_placement>(i);
		auto &resource = dev.get_memory_resource(placement);

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
	target_placement placement
) const
{
	const auto index = static_cast<std::size_t>(placement);
	auto *allocator = m_allocators_by_placement.at(index);
	XMIPP4_ASSERT(allocator);
	return *allocator;
}

} // namespace hardware
} // namespace xmipp4
