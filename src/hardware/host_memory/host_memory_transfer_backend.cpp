// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_transfer_backend.hpp"

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/memory_transfer_manager.hpp>

#include "host_memory_transfer.hpp"

namespace xmipp4
{
namespace hardware
{

backend_priority host_memory_transfer_backend::get_suitability(
    const memory_resource& source,
    const memory_resource& destination
) const noexcept
{

    if (
        is_host_accessible(source.get_kind()) && 
        is_host_accessible(destination.get_kind())
    ) 
    {
        return backend_priority::normal;
    }
    else
    {
        return backend_priority::unsupported;
    }
}

std::shared_ptr<memory_transfer> host_memory_transfer_backend::create_transfer(
    const memory_resource& source,
    const memory_resource& destination
) const
{
    if (!is_host_accessible(source.get_kind()))
    {
        throw std::invalid_argument(
            "source memory resource is not host accessible"
        );
    }
    
    if (!is_host_accessible(destination.get_kind())) 
    {
        throw std::invalid_argument(
            "destination memory resource is not host accessible"
        );
    }

    return std::make_shared<host_memory_transfer>();
}

bool host_memory_transfer_backend::register_at(memory_transfer_manager &manager)
{
    return manager.register_backend(
        std::make_unique<host_memory_transfer_backend>()
    );
}

} // namespace hardware
} // namespace xmipp4
