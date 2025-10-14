// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../device.hpp"

namespace xmipp4 
{
namespace compute
{

class cpu_event;
class cpu_device_queue;
class cpu_memory_resource;

/**
 * @brief Special implementation of the device interface to be able to use
 * the "host" as a device.
 * 
 */
class cpu_device final
    : public device
{
public:
    cpu_device() = default;
    ~cpu_device() override = default;

    void enumerate_memory_resources(
        std::vector<std::shared_ptr<memory_resource>> &resources
    ) override;

    std::shared_ptr<device_queue> create_device_queue() override;

    std::shared_ptr<device_event> create_device_event() override;

    std::shared_ptr<device_to_host_event> 
    create_device_to_host_event() override;

private:
    std::shared_ptr<cpu_memory_resource> m_memory_resource;
    std::shared_ptr<cpu_device_queue> m_queue;
    std::shared_ptr<cpu_event> m_event;

}; 

} // namespace compute
} // namespace xmipp4
