// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_device.hpp>

#include "../host_memory_transfer.hpp"

#include <xmipp4/core/compute/cpu/cpu_device_queue.hpp>
#include <xmipp4/core/compute/cpu/cpu_event.hpp>
#include <xmipp4/core/compute/memory_resource.hpp>

namespace xmipp4
{
namespace compute
{

void cpu_device::enumerate_memory_resources(
    std::vector<memory_resource*> &resources
)
{
    auto &host_memory_resource = get_host_memory_resource();
    resources = { &host_memory_resource };
}

std::shared_ptr<memory_transfer> cpu_device::create_memory_transfer(
    const memory_resource &source,
    const memory_resource &destination
)
{
    if (!is_host_accessible(source.get_kind()))
    {
        throw std::invalid_argument(
            "Source memory_resource is not host accessible."
        );
    }

    if (!is_host_accessible(destination.get_kind()))
    {
        throw std::invalid_argument(
            "Destination memory_resource is not host accessible."
        );
    }

    return std::make_shared<host_memory_transfer>();
}

std::shared_ptr<device_queue> cpu_device::create_device_queue()
{
    return std::make_shared<cpu_device_queue>();
}

std::shared_ptr<device_event> cpu_device::create_device_event()
{
    return std::make_shared<cpu_event>();
}

std::shared_ptr<device_to_host_event> 
cpu_device::create_device_to_host_event()
{
    return std::make_shared<cpu_event>();
}

} // namespace compute
} // namespace xmipp4
