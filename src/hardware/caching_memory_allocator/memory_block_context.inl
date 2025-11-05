// SPDX-License-Identifier: GPL-3.0-only

#include "memory_block_context.hpp"

namespace xmipp4
{
namespace hardware
{

inline
memory_block_context::memory_block_context(
    iterator prev, 
    iterator next, 
    bool free 
) noexcept
    : m_prev(prev)
    , m_next(next)
    , m_free(free)
{
}

inline
void memory_block_context::set_previous_block(iterator prev) noexcept
{
    m_prev = prev;
}

inline
memory_block_context::iterator 
memory_block_context::get_previous_block() const noexcept
{
    return m_prev;
}

inline
void memory_block_context::set_next_block(iterator next) noexcept
{
    m_next = next;
}

inline
memory_block_context::iterator
memory_block_context::get_next_block() const noexcept
{
    return m_next;
}

inline
void memory_block_context::set_free(bool free)
{
    m_free = free;
}

inline
bool memory_block_context::is_free() const noexcept
{
    return m_free;
}



inline
bool is_partition(const memory_block_context &block) noexcept
{
    const memory_block_context::iterator null;
    return block.get_previous_block() != null ||
           block.get_next_block() != null ;
}

} // namespace hardware
} // namespace xmipp4
