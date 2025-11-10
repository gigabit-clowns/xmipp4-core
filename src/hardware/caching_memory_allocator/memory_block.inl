// SPDX-License-Identifier: GPL-3.0-only

#include "memory_block.hpp"

#include <xmipp4/core/memory/align.hpp>

namespace xmipp4
{
namespace hardware
{

inline
memory_block::memory_block(
    std::shared_ptr<memory_heap> heap,
    std::size_t offset,
    std::size_t size, 
    const device_queue *queue
) noexcept
    : m_queue(queue)
    , m_size(size)
    , m_heap(std::move(heap))
    , m_offset(offset)
{
}

inline
memory_heap* memory_block::get_heap() const noexcept
{
    return m_heap.get();
}

inline
std::shared_ptr<memory_heap> memory_block::share_heap() const noexcept
{
    return m_heap;
}

inline
std::size_t memory_block::get_offset() const noexcept
{
    return m_offset;
}

inline
std::size_t memory_block::get_size() const noexcept
{
    return m_size;
}

inline
const device_queue* memory_block::get_queue() const noexcept
{
    return m_queue;
}



inline
bool operator==(const memory_block &lhs, const memory_block &rhs) noexcept
{
    return 
        lhs.get_heap() == rhs.get_heap() &&
        lhs.get_offset() == rhs.get_offset() &&
        lhs.get_size() == rhs.get_size() &&
        lhs.get_queue() == rhs.get_queue() ;

}

inline
bool operator!=(const memory_block &lhs, const memory_block &rhs) noexcept
{
    return !(lhs == rhs);
}



inline
bool memory_block_less::operator()(
    const memory_block &lhs, 
    const memory_block &rhs 
) const noexcept
{
    return as_tuple(lhs) < as_tuple(rhs);
}

inline
memory_block_less::tuple_type 
memory_block_less::as_tuple(const memory_block &block) noexcept
{
    return tuple_type(
        block.get_queue(),
        block.get_size(),
        block.get_heap(),
        block.get_offset()
    );
}

} // namespace hardware
} // namespace xmipp4
