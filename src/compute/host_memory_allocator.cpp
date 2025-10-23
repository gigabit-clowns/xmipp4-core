// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_allocator.hpp"

#include "host_buffer.hpp"
#include "host_memory_resource.hpp"

#include <xmipp4/core/compute/device_queue.hpp>

#include <stdexcept>

namespace xmipp4
{
namespace compute
{

memory_resource& host_memory_allocator::get_memory_resource() const noexcept
{
    return host_memory_resource::get();
}

std::shared_ptr<buffer> host_memory_allocator::allocate(
    std::size_t size, 
    std::size_t alignment, 
    device_queue *queue
)
{
    if (queue)
    {
        // As host allocation is synchronous, we need to wait until all previous
        // operations in the provided queue are finished.
        queue->wait_until_completed();
    }

    return std::make_shared<host_buffer>(size, alignment);
}

} // namespace compute
} // namespace xmipp4
