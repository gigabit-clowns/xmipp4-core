// SPDX-License-Identifier: GPL-3.0-only

#include "memory_block_pool.hpp"

#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/hardware/memory_heap.hpp>

namespace xmipp4
{
namespace hardware
{

static
bool is_mergeable(memory_block_pool::iterator ite) noexcept
{
    bool result;

    if (ite != memory_block_pool::iterator())
    {
        result = ite->second.is_free();
    }
    else
    {
        result = false;
    }

    return result;
}

static
void update_forward_link(memory_block_pool::iterator ite) noexcept
{
    const auto next = ite->second.get_next_block();
    if (next != memory_block_pool::iterator())
    {
        next->second.set_previous_block(ite);
    }
}

static
void update_backward_link(memory_block_pool::iterator ite) noexcept
{
    const auto prev = ite->second.get_previous_block();
    if (prev != memory_block_pool::iterator())
    {
        prev->second.set_next_block(ite);
    }
}

static
void update_links(memory_block_pool::iterator ite) noexcept
{
    update_backward_link(ite);
    update_forward_link(ite);
}

static
bool check_forward_link(memory_block_pool::iterator ite)  noexcept
{
    bool result;

    const auto next = ite->second.get_next_block();
    if (next != memory_block_pool::iterator())
    {
        result = next->second.get_previous_block() == ite;
    }
    else
    {
        result = true;
    }

    return result;
}

static
bool check_backward_link(memory_block_pool::iterator ite) noexcept
{
    bool result;

    const auto prev = ite->second.get_previous_block();
    if (prev != memory_block_pool::iterator())
    {
        result = prev->second.get_next_block() == ite;
    }
    else
    {
        result = true;
    }

    return result;
}

static
bool check_links(memory_block_pool::iterator ite) noexcept
{
    return check_backward_link(ite) && check_forward_link(ite);
}



memory_block_pool::iterator memory_block_pool::begin()
{
    return m_blocks.begin();
}

memory_block_pool::iterator memory_block_pool::end()
{
    return m_blocks.end();
}

memory_block_pool::iterator memory_block_pool::find_suitable_block(
    std::size_t size,
    const device_queue *queue 
)
{
    // Assuming that the blocks are ordered according to their queue reference
    // first and then their sizes, the best fit is achieved iterating from
    // the first suitable block.
    const memory_block key(nullptr, 0UL, size, queue);
    auto ite = m_blocks.lower_bound(key);

    while (ite != m_blocks.end())
    {
        if(ite->first.get_queue() != queue)
        {
            // Reached the end of the allowed range.
            ite = m_blocks.end();
        }
        else if (ite->second.is_free())
        {
            // Found a suitable block
            break;
        }
        else
        {
            // Occupied. Try with the next.
            ++ite;
        }
    }

    return ite;
}

std::pair<memory_block_pool::iterator, memory_block_pool::iterator>
memory_block_pool::partition_block(
    iterator ite,
    std::size_t size,
    std::size_t remaining
)
{
    auto heap = ite->first.share_heap();
    const auto base_offset = ite->first.get_offset();
    const auto queue = ite->first.get_queue();
    const auto prev = ite->second.get_previous_block();
    const auto next = ite->second.get_next_block();

    // Remove old block
    m_blocks.erase(ite);

    iterator first;
    iterator second;
    bool inserted;
    std::tie(first, inserted) = m_blocks.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(
            heap,
            base_offset, 
            size, 
            queue
        ),
        std::forward_as_tuple(
            prev,
            memory_block_pool::iterator(), // To be set
            ite->second.is_free()
        )
    );
    XMIPP4_ASSERT(inserted);
    std::tie(second, inserted) = m_blocks.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(
            std::move(heap), // No longer required
            base_offset + size, 
            remaining, 
            queue
        ),
        std::forward_as_tuple(
            first,
            next,
            ite->second.is_free()
        )
    );
    XMIPP4_ASSERT(inserted);
    
    first->second.set_next_block(second);
    update_backward_link(first);
    update_forward_link(second);

    XMIPP4_ASSERT( check_links(first) );
    XMIPP4_ASSERT( check_links(second) );

    return std::make_pair(first, second);
}

memory_block_pool::iterator memory_block_pool::register_heap(
    std::shared_ptr<memory_heap> heap, 
    const device_queue *queue
)
{
    iterator result;
    bool inserted;

    if (!heap)
    {
        throw std::invalid_argument("Provided null heap");
    }

    const memory_block_pool::iterator null;
    std::tie(result, inserted) = m_blocks.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(std::move(heap), 0UL, heap->get_size(), queue),
        std::forward_as_tuple(null, null, true)
    );

    if (!inserted)
    {
        throw std::invalid_argument("Provided block is already in the pool");
    }

    return result;
}

memory_block_pool::iterator 
memory_block_pool::consider_merging_block(
    memory_block_pool::iterator ite
)
{
    const auto prev = ite->second.get_previous_block();
    const auto merge_prev = is_mergeable(prev);
    const auto next = ite->second.get_next_block();
    const auto merge_next = is_mergeable(next);

    if (merge_prev && merge_next)
    {
        ite = merge_blocks(prev, ite, next);
    }
    else if (merge_prev)
    {
        ite = merge_blocks(prev, ite);
    }
    else if (merge_next)
    {   
        ite = merge_blocks(ite, next);
    }

    return ite;
}

void memory_block_pool::release_blocks()
{
    auto ite = m_blocks.begin();
    while (ite != m_blocks.cend())
    {
        if(ite->second.is_free() && !is_partition(ite->second))
        {
            ite = m_blocks.erase(ite);
        }
        else
        {
            ++ite;
        }
    }
}

memory_block_pool::iterator
memory_block_pool::merge_blocks(
    iterator first,
    iterator second
)
{
    XMIPP4_ASSERT( first->second.is_free() );
    XMIPP4_ASSERT( second->second.is_free() );
    XMIPP4_ASSERT( first->second.get_next_block() == second );
    XMIPP4_ASSERT( second->second.get_previous_block() == first );

    const auto heap = first->first.share_heap();
    const auto offset = first->first.get_offset();
    const auto size = first->first.get_size() +
                      second->first.get_size() ;
    const auto queue = first->first.get_queue();
    const auto prev = first->second.get_previous_block();
    const auto next = second->second.get_next_block();

    iterator ite;
    bool inserted;
    std::tie(ite, inserted) = m_blocks.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(std::move(heap), offset, size, queue),
        std::forward_as_tuple(prev, next, true)
    );
    XMIPP4_ASSERT(inserted);

    update_links(ite);

    m_blocks.erase(first);
    m_blocks.erase(second);

    XMIPP4_ASSERT( check_links(ite) );
    return ite;
}

memory_block_pool::iterator
memory_block_pool::merge_blocks(
    iterator first,
    iterator second,
    iterator third
)
{
    XMIPP4_ASSERT( first->second.is_free() );
    XMIPP4_ASSERT( second->second.is_free() );
    XMIPP4_ASSERT( third->second.is_free() );
    XMIPP4_ASSERT( first->second.get_next_block() == second );
    XMIPP4_ASSERT( second->second.get_previous_block() == first );
    XMIPP4_ASSERT( second->second.get_next_block() == third );
    XMIPP4_ASSERT( third->second.get_previous_block() == second );

    auto heap = first->first.share_heap();
    const auto offset = first->first.get_offset();
    const auto size = first->first.get_size() +
                      second->first.get_size() +
                      third->first.get_size() ;
    const auto queue = first->first.get_queue();
    const auto prev = first->second.get_previous_block();
    const auto next = third->second.get_next_block();

    memory_block_pool::iterator ite;
    bool inserted;
    std::tie(ite, inserted) = m_blocks.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(std::move(heap), offset, size, queue),
        std::forward_as_tuple(prev, next, true)
    );
    XMIPP4_ASSERT(inserted);

    update_links(ite);

    m_blocks.erase(first);
    m_blocks.erase(second);
    m_blocks.erase(third);

    XMIPP4_ASSERT( check_links(ite) );
    return ite;
}

} // namespace hardware
} // namespace xmipp4
