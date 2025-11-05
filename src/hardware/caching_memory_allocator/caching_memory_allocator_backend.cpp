// SPDX-License-Identifier: GPL-3.0-only

#include "caching_memory_allocator_backend.hpp"

#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include "caching_memory_allocator.hpp"

namespace xmipp4
{
namespace hardware
{

backend_priority caching_memory_allocator_backend::get_priority(
    memory_resource&
) const noexcept
{
    return backend_priority::fallback;
}

std::shared_ptr<memory_allocator> 
caching_memory_allocator_backend::create_memory_allocator(
    memory_resource &resource
) const
{
    return std::make_shared<caching_memory_allocator>(
        resource,
        256,
        1<<20 //2MB
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
