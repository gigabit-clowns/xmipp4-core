// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_resource.hpp"

#include "host_memory_transfer.hpp"
#include "host_memory_heap.hpp"

namespace xmipp4
{
namespace hardware
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

std::shared_ptr<memory_heap> 
host_memory_resource::create_memory_heap(std::size_t size)
{
    return std::make_shared<host_memory_heap>(size);
}

host_memory_resource& host_memory_resource::get() noexcept
{
    return m_instance;
}

} // namespace hardware
} // namespace xmipp4
