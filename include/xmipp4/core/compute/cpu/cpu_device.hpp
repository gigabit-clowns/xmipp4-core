// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "../device.hpp"

#include "cpu_device_queue_pool.hpp"

namespace xmipp4 
{
namespace compute
{

class cpu_unified_memory_allocator;
class cpu_unified_copy;
class cpu_event;

/**
 * @brief Special implementation of the device interface to be able to use
 * the "host" as a device.
 * 
 */
class cpu_device final
    : public device
{
public:
    cpu_device();
    ~cpu_device() override = default;

    cpu_device_queue_pool& get_queue_pool() override;

    std::shared_ptr<device_memory_allocator> 
    create_device_memory_allocator() override;

    std::shared_ptr<host_memory_allocator> 
    create_host_memory_allocator() override;

    std::shared_ptr<host_to_device_transfer> 
    create_host_to_device_transfer() override;

    std::shared_ptr<device_to_host_transfer> 
    create_device_to_host_transfer() override;

    std::shared_ptr<device_copy> 
    create_device_copy() override;

    std::shared_ptr<device_event> create_device_event() override;

    std::shared_ptr<device_to_host_event> 
    create_device_to_host_event() override;

private:
    cpu_device_queue_pool m_queue_pool;
    std::shared_ptr<cpu_unified_memory_allocator> m_allocator;
    std::shared_ptr<cpu_unified_copy> m_transfer;
    std::shared_ptr<cpu_event> m_event;

}; 

} // namespace compute
} // namespace xmipp4
