// SPDX-License-Identifier: GPL-3.0-only

#include "caching_memory_allocator.hpp"

#include "caching_memory_sentinel.hpp"

#include <xmipp4/core/logger.hpp>
#include <xmipp4/core/hardware/memory_heap.hpp>

namespace xmipp4
{
namespace hardware
{

caching_memory_allocator::caching_memory_allocator(
    memory_resource &resource,
    std::size_t maximum_alignment,
    std::size_t heap_size_step    
)
    : m_resource(resource)
    , m_device(resource.get_target_device())
    , m_maximum_alignment(maximum_alignment)
    , m_heap_size_step(heap_size_step)
{
}

caching_memory_allocator::~caching_memory_allocator()
{
    try
    {
        m_deferred_release.wait_pending_free(m_pool);
    }
    catch(...)
    {
        // Can not throw on destructor. Catch exceptions and report.
        XMIPP4_LOG_ERROR(
            "An exception occurred waiting pending releases in "
            "~caching_memory_allocator()"
        );
    }
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
    if (alignment > m_maximum_alignment)
    {
        throw std::invalid_argument(
            "alignment parameter exceeds the maximum alignment of this "
            "allocator"
        );
    }

    size = memory::align_ceil(size, m_maximum_alignment);

    m_deferred_release.process_pending_free(m_pool);
    auto ite = m_pool.find_suitable_block(size, alignment, queue);

    if (ite == m_pool.end())
    {
        // No suitable block was found in the pool. Request more memory.
        const auto request_size = memory::align_ceil(size, m_heap_size_step);
        std::shared_ptr<memory_heap> heap;
        try
        {
            heap = get_memory_resource().create_memory_heap(request_size);
        }
        catch (const std::bad_alloc&)
        {
            XMIPP4_LOG_WARN(
                "Memory allocation failed. Retrying after releasing  resources."
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
    if (remaining >= m_maximum_alignment)
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

    return create_buffer(ite);
}

void caching_memory_allocator::recycle_block(
    memory_block_pool::iterator block, 
    span<device_queue *const> queues
)
{
    if (queues.empty())
    {
        m_pool.recycle_block(block);
    }
    else if (m_device)
    {
        m_deferred_release.defer_release(block, queues, *m_device);
    }
    else
    {
        for (device_queue *queue : queues)
        {
            if (!queue)
            {
                throw std::invalid_argument(
                    "nullptr queue can not be provided to recycle block"
                );
            }

            queue->wait_until_completed();
        }
    }
}

std::shared_ptr<buffer> 
caching_memory_allocator::create_buffer(memory_block_pool::iterator block)
{
    auto *heap = block->first.get_heap();
    const auto offset = block->first.get_offset();
    const auto size = block->first.get_size();

    auto tracker = 
        std::make_unique<caching_memory_sentinel>(*this, block);

    XMIPP4_ASSERT(heap);
    return heap->create_buffer(offset, size, std::move(tracker));
}

} // namespace hardware
} // namespace xmipp4
