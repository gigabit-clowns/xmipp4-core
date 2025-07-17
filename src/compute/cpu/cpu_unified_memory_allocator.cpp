// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file cpu_unified_memory_allocator.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of cpu_unified_memory_allocator.hpp
 * @date 2024-11-06
 * 
 */

#include <xmipp4/core/compute/cpu/cpu_unified_memory_allocator.hpp>

#include "default_cpu_unified_buffer.hpp"
#include <xmipp4/core/compute/cpu/cpu_device_queue.hpp>

namespace xmipp4
{
namespace compute
{

std::shared_ptr<cpu_unified_buffer> 
cpu_unified_memory_allocator::create_unified_buffer(std::size_t size, 
                                                     std::size_t alignment )
{
    return std::make_shared<default_cpu_unified_buffer>(size, alignment);
}

std::shared_ptr<device_buffer> 
cpu_unified_memory_allocator::create_device_buffer(std::size_t size,
                                                    std::size_t alignment,
                                                    device_queue& )
{
    return create_unified_buffer(size, alignment);
}

std::shared_ptr<host_buffer> 
cpu_unified_memory_allocator::create_host_buffer(std::size_t size,
                                                  std::size_t alignment,
                                                  device_queue& )
{
    return create_unified_buffer(size, alignment);
}

std::shared_ptr<host_buffer> 
cpu_unified_memory_allocator::create_host_buffer(std::size_t size,
                                                  std::size_t alignment )
{
    return create_unified_buffer(size, alignment);
}

} // namespace compute
} // namespace xmipp4
