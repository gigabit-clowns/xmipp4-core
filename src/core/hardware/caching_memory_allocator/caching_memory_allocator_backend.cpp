// SPDX-License-Identifier: GPL-3.0-only

#include "caching_memory_allocator_backend.hpp"

#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include "caching_memory_allocator.hpp"

#include <algorithm>

namespace xmipp4
{
namespace hardware
{

backend_priority caching_memory_allocator_backend::get_suitability(
	const memory_resource&
) const noexcept
{
	return backend_priority::normal;
}

std::shared_ptr<memory_allocator> 
caching_memory_allocator_backend::create_memory_allocator(
	memory_resource &resource
) const
{
	const auto max_alignment = std::min(
		resource.get_max_heap_alignment(), 
		std::size_t(256)
	);
	const auto request_size_step = std::size_t(64) << 20; // 64MB

	return std::make_shared<caching_memory_allocator>(
		resource,
		max_alignment,
		request_size_step
	);
}

bool caching_memory_allocator_backend::register_at(
	memory_allocator_manager &manager
)
{
	return manager.register_backend(
		std::make_unique<caching_memory_allocator_backend>()
	);
}

} // namespace hardware
} // namespace xmipp4
