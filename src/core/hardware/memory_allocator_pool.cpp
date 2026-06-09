// SPDX-License-Identifier: GPL-3.0-only

#include "memory_allocator_pool.hpp"

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/platform/assert.hpp>

namespace xmipp4
{
namespace hardware
{

const std::shared_ptr<memory_allocator>& 
memory_allocator_pool::touch(memory_resource &resource)
{
	auto ite = m_allocators.find(&resource);
	if (ite == m_allocators.end())
	{
		
		std::tie(ite, std::ignore) = m_allocators.emplace(
			&resource,
			resource.create_allocator()
		);
	}

	XMIPP4_ASSERT( ite != m_allocators.end() );
	return ite->second;
}

} // namespace hardware
} // namespace xmipp4

