// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file cpu_unified_memory_allocator.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::cpu_unified_memory_allocator class
 * @date 2024-11-06
 * 
 */


#include "../device_memory_allocator.hpp"
#include "../host_memory_allocator.hpp"


namespace xmipp4 
{
namespace compute
{

class cpu_unified_buffer;

/**
 * @brief Special implementation of device_memory_allocator and
 * host_memory_allocator that allows to allocate memory in the host 
 * as if it were a device.
 * 
 */
class cpu_unified_memory_allocator
    : public device_memory_allocator
    , public host_memory_allocator
{
public:
    cpu_unified_memory_allocator() = default;
    cpu_unified_memory_allocator(const cpu_unified_memory_allocator &other) = default;
    cpu_unified_memory_allocator(cpu_unified_memory_allocator &&other) = default;
    ~cpu_unified_memory_allocator() override = default;

    cpu_unified_memory_allocator& 
    operator=(const cpu_unified_memory_allocator &other) = default;
    cpu_unified_memory_allocator& 
    operator=(cpu_unified_memory_allocator &&other) = default;

    std::shared_ptr<cpu_unified_buffer> 
    create_unified_buffer(std::size_t size, std::size_t alignment);

    std::shared_ptr<device_buffer> 
    create_device_buffer(std::size_t size, 
                         std::size_t alignment,
                         device_queue &queue ) override;

    std::shared_ptr<host_buffer> 
    create_host_buffer(std::size_t size, 
                       std::size_t alignment,
                       device_queue &queue ) override;

    std::shared_ptr<host_buffer> 
    create_host_buffer(std::size_t size, std::size_t alignment) override;

}; 

} // namespace compute
} // namespace xmipp4
