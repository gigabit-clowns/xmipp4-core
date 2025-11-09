// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_transfer_backend.hpp"

#include <xmipp4/core/hardware/memory_resource.hpp>
#include <xmipp4/core/hardware/memory_transfer_manager.hpp>

#include "host_memory_transfer.hpp"

namespace xmipp4
{
namespace hardware
{

std::shared_ptr<memory_transfer> host_memory_transfer_backend::create_transfer(
    const memory_resource& src,
    const memory_resource& dst
) const
{
    if (is_host_accessible(src.get_kind()) && is_host_accessible(dst.get_kind())) 
    {
        return std::make_shared<host_memory_transfer>();
    }

    return nullptr;
}

bool host_memory_transfer_backend::register_at(memory_transfer_manager &manager)
{
    return manager.register_backend(
        std::make_unique<host_memory_transfer_backend>()
    );
}

} // namespace hardware
} // namespace xmipp4
