// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_unified_memory_allocator.hpp>

#include <xmipp4/core/compute/cpu/cpu_unified_buffer.hpp>
#include <xmipp4/core/compute/cpu/cpu_device_queue.hpp>

namespace xmipp4
{
namespace compute
{

std::shared_ptr<cpu_unified_buffer> 
cpu_unified_memory_allocator::create_unified_buffer_impl(std::size_t size, 
                                                         std::size_t alignment )
{
    return std::make_shared<cpu_unified_buffer>(size, alignment);
}

std::shared_ptr<unified_buffer> 
cpu_unified_memory_allocator::create_unified_buffer(std::size_t size, 
                                                    std::size_t alignment )
{
    return create_unified_buffer_impl(size, alignment);
}

std::shared_ptr<unified_buffer> 
cpu_unified_memory_allocator::create_unified_buffer(std::size_t size, 
                                                    std::size_t alignment,
                                                    device_queue &queue )
{
    std::ignore = dynamic_cast<cpu_device_queue&>(queue);
    return create_unified_buffer_impl(size, alignment);
}

} // namespace compute
} // namespace xmipp4

