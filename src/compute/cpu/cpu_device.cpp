// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_device.hpp>

#include <xmipp4/core/compute/cpu/cpu_memory_resource.hpp>
#include <xmipp4/core/compute/cpu/cpu_device_queue.hpp>
#include <xmipp4/core/compute/cpu/cpu_event.hpp>

namespace xmipp4
{
namespace compute
{

void cpu_device::enumerate_memory_resources(
    std::vector<std::shared_ptr<memory_resource>> &resources
)
{
    if (!m_memory_resource)
    {
        m_memory_resource = std::make_shared<cpu_memory_resource>();
    }

    resources = { m_memory_resource };
}

std::shared_ptr<device_queue> cpu_device::create_device_queue()
{
    if (!m_queue)
    {
        m_queue = std::make_shared<cpu_device_queue>();
    }

    return m_queue; // Stateless. Return always the same.
}

std::shared_ptr<device_event> cpu_device::create_device_event()
{
    if (!m_event)
    {
        m_event = std::make_shared<cpu_event>();
    }
    
    return m_event; // Stateless. Return always the same.
}

std::shared_ptr<device_to_host_event> 
cpu_device::create_device_to_host_event()
{
    if (!m_event)
    {
        m_event = std::make_shared<cpu_event>();
    }

    return m_event; // Stateless. Return always the same.
}

} // namespace compute
} // namespace xmipp4
