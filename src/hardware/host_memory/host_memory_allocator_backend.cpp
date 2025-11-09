// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_allocator_backend.hpp"

#include <xmipp4/core/hardware/memory_allocator_manager.hpp>

#include "host_memory_allocator.hpp"
#include "host_memory_resource.hpp"

namespace xmipp4
{
namespace hardware
{

static bool is_host_memory_resource(const memory_resource &resource)
{
    // Checks for the same address as the host memory resource
    // is a singleton
    return &resource == &host_memory_resource::get();
}

backend_priority host_memory_allocator_backend::get_priority(
    const memory_resource &resource
) const noexcept
{
    if (is_host_memory_resource(resource))
    {
        return backend_priority::optimal;
    }
    else
    {
        return backend_priority::unsupported;
    }
}

std::shared_ptr<memory_allocator> 
host_memory_allocator_backend::create_memory_allocator(
    memory_resource &resource
) const
{
    if(!is_host_memory_resource(resource))
    {
        throw std::invalid_argument(
            "Can not provide an allocator for a memory resource other than "
            "host_memory_resource"
        );
    }

    return std::make_shared<host_memory_allocator>();
}


bool host_memory_allocator_backend::register_at(
    memory_allocator_manager &manager
)
{
    return manager.register_backend(
        std::make_unique<host_memory_allocator_backend>()
    );
}

} // namespace hardware
} // namespace xmipp4
