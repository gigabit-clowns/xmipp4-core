// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/unified_memory_allocator.hpp>

#include <xmipp4/core/compute/unified_buffer.hpp>

namespace xmipp4
{
namespace compute
{

std::shared_ptr<device_buffer> 
unified_memory_allocator::create_device_buffer(std::size_t size,
                                               std::size_t alignment,
                                               device_queue& queue)
{
    return create_unified_buffer(size, alignment, queue);
}

std::shared_ptr<host_buffer> 
unified_memory_allocator::create_host_buffer(std::size_t size,
                                             std::size_t alignment,
                                             device_queue& queue)
{
    return create_unified_buffer(size, alignment, queue);
}

std::shared_ptr<host_buffer> 
unified_memory_allocator::create_host_buffer(std::size_t size,
                                             std::size_t alignment )
{
    return create_unified_buffer(size, alignment);
}

} // namespace compute
} // namespace xmipp4
