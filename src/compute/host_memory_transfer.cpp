// SPDX-License-Identifier: GPL-3.0-only

#include "host_memory_transfer.hpp"

#include <xmipp4/core/compute/buffer.hpp>
#include <xmipp4/core/compute/copy_region.hpp>
#include <xmipp4/core/memory/byte.hpp>

#include <stdexcept>
#include <cstring>

namespace xmipp4
{
namespace compute
{

host_memory_transfer host_memory_transfer::m_instance;

void host_memory_transfer::copy(
    const buffer &source, 
    buffer &destination,
    span<const copy_region> regions, 
    device_queue *queue
) const
{
    if (queue)
    {
        throw std::invalid_argument(
            "host_memory_transfer does not support device queues."
        );
    }

    const auto *src_ptr = static_cast<const memory::byte*>(source.get_host_ptr());
    if (!src_ptr)
    {
        throw std::invalid_argument(
            "Source buffer is not host accessible."
        );
    }

    auto *dst_ptr = static_cast<memory::byte*>(destination.get_host_ptr());
    if (!dst_ptr)
    {
        throw std::invalid_argument(
            "Destination buffer is not host accessible."
        );
    }

    for (const auto &region : regions)
    {
        const auto src_offset = region.get_source_offset();
        const auto dst_offset = region.get_destination_offset();
        const auto byte_count = region.get_count();

        if (src_offset + byte_count > source.get_size())
        {
            throw std::out_of_range(
                "Copy region exceeds source buffer size."
            );
        }

        if (dst_offset + byte_count > destination.get_size())
        {
            throw std::out_of_range(
                "Copy region exceeds destination buffer size."
            );
        }

        std::memcpy(dst_ptr+dst_offset, src_ptr+src_offset, byte_count);
    }
}
    
const host_memory_transfer& host_memory_transfer::get() noexcept
{
    return m_instance;
}

} // namespace compute
} // namespace xmipp4
