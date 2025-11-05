// SPDX-License-Identifier: GPL-3.0-only

#include "caching_memory_allocator.hpp"

#include <xmipp4/core/logger.hpp>
#include <xmipp4/core/hardware/memory_heap.hpp>
#include <xmipp4/core/hardware/memory_allocation_tracker.hpp>

namespace xmipp4
{
namespace hardware
{

static
std::shared_ptr<buffer> create_buffer(const memory_block &block)
{
    auto *heap = block.get_heap();
    const auto offset = block.get_offset();
    const auto size = block.get_size();

    XMIPP4_ASSERT(heap);
    return heap->create_buffer(offset, size, nullptr);
}

caching_memory_allocator::caching_memory_allocator(
    memory_resource &resource,
    std::size_t size_step,
    std::size_t request_size_step    
)
    : m_resource(resource)
    , m_size_step(size_step)
    , m_request_size_step(request_size_step)
{
}

memory_resource& caching_memory_allocator::get_memory_resource() const noexcept
{
    return m_resource;
}

std::shared_ptr<buffer> caching_memory_allocator::allocate(
    std::size_t size, 
    std::size_t alignment, 
    device_queue *queue
)
{
    size = memory::align_ceil(size, m_size_step);

    m_deferred_release.process_pending_free(m_pool);
    auto ite = m_pool.find_suitable_block(size, alignment, queue);
    // TODO If failure, try to steal from queue-less pool.

    if (ite == m_pool.end())
    {
        // No suitable block was found in the pool. Request more memory.
        const auto request_size = memory::align_ceil(size, m_request_size_step);

        std::shared_ptr<memory_heap> heap;
        try
        {
            heap = get_memory_resource().create_memory_heap(request_size);
        }
        catch (const std::bad_alloc&)
        {
            XMIPP4_LOG_WARN(
                "Memory allocation failed. Retrying after releasing "
                "cached resources."
            );
            m_deferred_release.wait_pending_free(m_pool);
            m_pool.release_blocks();
            heap = get_memory_resource().create_memory_heap(request_size);
        }

        XMIPP4_ASSERT( heap );
        ite = m_pool.register_heap(std::move(heap), queue);
    }

    XMIPP4_ASSERT( ite != m_pool.end() );

    const auto block_size = ite->first.get_size();
    XMIPP4_ASSERT( block_size >= size );
    const auto remaining = block_size - size;
    if (remaining >= m_size_step)
    {
        // Only partition if the block has a reminder larger than the 
        // step size.
        std::tie(ite, std::ignore) = m_pool.partition_block(
            ite, 
            size, 
            remaining
        ); 
    }

    // Mark the block as occupied.
    ite->second.set_free(false);

    return create_buffer(ite->first);
}

} // namespace hardware
} // namespace xmipp4
