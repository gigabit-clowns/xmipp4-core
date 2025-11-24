// SPDX-License-Identifier: GPL-3.0-only

#include "memory_block.hpp"

#include <xmipp4/core/memory/align.hpp>

namespace xmipp4
{
namespace hardware
{

inline
memory_block::memory_block(
	const device_queue *queue,
	std::size_t size, 
	memory_heap *heap,
	std::size_t offset
) noexcept
	: m_queue(queue)
	, m_size(size)
	, m_heap(std::move(heap))
	, m_offset(offset)
{
}

inline
const device_queue* memory_block::get_queue() const noexcept
{
	return m_queue;
}

inline
void memory_block::set_size(std::size_t size) noexcept
{
	m_size = size;
}

inline
std::size_t memory_block::get_size() const noexcept
{
	return m_size;
}

inline
memory_heap* memory_block::get_heap() const noexcept
{
	return m_heap;
}

inline
void memory_block::set_offset(std::size_t offset) noexcept
{
	m_offset = offset;
}

inline
std::size_t memory_block::get_offset() const noexcept
{
	return m_offset;
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

} // namespace hardware
} // namespace xmipp4
