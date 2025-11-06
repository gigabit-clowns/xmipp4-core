// SPDX-License-Identifier: GPL-3.0-only

#include "caching_memory_allocation_tracker.hpp"

#include "caching_memory_allocator.hpp"


#include <algorithm>

namespace xmipp4
{
namespace hardware
{

caching_memory_allocation_tracker::caching_memory_allocation_tracker(
    caching_memory_allocator &allocator,
    memory_block_pool::iterator block
)
    : m_allocator(allocator)
    , m_block(block)
{
}

caching_memory_allocation_tracker::~caching_memory_allocation_tracker()
{
    m_allocator.get().recycle_block(m_block, get_queues());
}

span<device_queue *const> 
caching_memory_allocation_tracker::get_queues() const noexcept
{
    return make_span(m_queues);
}

void caching_memory_allocation_tracker::record_queue(device_queue &queue, bool)
{
    auto *const queue_pointer = &queue;
    if (queue_pointer != m_block->first.get_queue())
    {
        // Find first element that compares greater or EQUAL.
        const auto ite = std::lower_bound(
            m_queues.cbegin(), m_queues.cend(),
            queue_pointer
        );

        // Ensure that it is not equal.
        if (ite == m_queues.cend() || *ite != queue_pointer)
        {
            m_queues.insert(ite, queue_pointer);
        }
    }
}

} // namespace hardware
} // namespace xmipp4
