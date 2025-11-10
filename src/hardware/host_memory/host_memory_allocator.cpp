// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_allocator.hpp"

#include <xmipp4/core/hardware/buffer.hpp>
#include <xmipp4/core/memory/align.hpp>

#include "host_memory_resource.hpp"
#include "host_buffer.hpp"

#include <stdexcept>

namespace xmipp4
{
namespace hardware
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
        throw std::invalid_argument(
            "host_memory_allocator does not support queue execution"
        );
    }

    size = memory::align_ceil(size, alignment);
    return std::make_shared<host_buffer>(size, alignment);
}

} // namespace hardware
} // namespace xmipp4
