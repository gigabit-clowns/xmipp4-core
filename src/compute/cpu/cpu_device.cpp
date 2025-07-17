// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_device.hpp>

#include <xmipp4/core/compute/cpu/cpu_device_queue.hpp>
#include <xmipp4/core/compute/cpu/cpu_unified_memory_allocator.hpp>
#include <xmipp4/core/compute/cpu/cpu_transfer.hpp>
#include <xmipp4/core/compute/cpu/cpu_event.hpp>

namespace xmipp4
{
namespace compute
{

cpu_device::cpu_device()
    : m_queue_pool()
    , m_allocator(std::make_shared<cpu_unified_memory_allocator>())
    , m_transfer(std::make_shared<cpu_transfer>())
    , m_event(std::make_shared<cpu_event>())
{
}

cpu_device_queue_pool& cpu_device::get_queue_pool()
{
    return m_queue_pool;
}

std::shared_ptr<device_memory_allocator> 
cpu_device::create_device_memory_allocator() 
{
    return m_allocator;
}

std::shared_ptr<host_memory_allocator> 
cpu_device::create_host_memory_allocator() 
{
    return m_allocator;
}

std::shared_ptr<host_to_device_transfer> 
cpu_device::create_host_to_device_transfer()
{
    return m_transfer;
}

std::shared_ptr<device_to_host_transfer> 
cpu_device::create_device_to_host_transfer()
{
    return m_transfer;
}

std::shared_ptr<device_copy> 
cpu_device::create_device_copy()
{
    return m_transfer;
}

std::shared_ptr<device_event> cpu_device::create_device_event()
{
    return m_event;
}

std::shared_ptr<device_to_host_event> 
cpu_device::create_device_to_host_event()
{
    return m_event;
}

} // namespace compute
} // namespace xmipp4
