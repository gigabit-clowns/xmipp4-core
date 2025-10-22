// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_resource.hpp"

#include "host_memory_transfer.hpp"
#include "host_memory_allocator.hpp"

namespace xmipp4
{
namespace compute
{

host_memory_resource host_memory_resource::m_instance;

device* host_memory_resource::get_target_device() const noexcept
{
    return nullptr;
}

memory_resource_kind host_memory_resource::get_kind() const noexcept
{
    return memory_resource_kind::host;
}

std::shared_ptr<memory_allocator> host_memory_resource::create_allocator()
{
    return std::make_shared<host_memory_allocator>();
}

const memory_transfer& host_memory_resource::get_intra_resource_transfer() const
{
    return host_memory_transfer::get();
}

host_memory_resource& host_memory_resource::get() noexcept
{
    return m_instance;
}

} // namespace compute
} // namespace xmipp4
