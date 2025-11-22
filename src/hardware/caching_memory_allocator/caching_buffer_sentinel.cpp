// SPDX-License-Identifier: GPL-3.0-only

#include "caching_buffer_sentinel.hpp"

#include "caching_memory_allocator.hpp"


#include <algorithm>

namespace xmipp4
{
namespace hardware
{

caching_buffer_sentinel::caching_buffer_sentinel(
	caching_memory_allocator &allocator,
	memory_block_pool::iterator block
)
	: m_allocator(allocator)
	, m_block(block)
{
}

caching_buffer_sentinel::~caching_buffer_sentinel()
{
	m_allocator.get().recycle_block(m_block, get_queues());
}

span<device_queue *const> 
caching_buffer_sentinel::get_queues() const noexcept
{
	const auto &queues = m_queues.get_sequence_cref();
	return span<device_queue *const>(queues.data(), queues.size());
}

void caching_buffer_sentinel::record_queue(device_queue &queue, bool exclusive)
{
	if (exclusive)
	{
		m_queues.clear();
	}

	auto *const queue_pointer = &queue;
	if (queue_pointer != m_block->first.get_queue())
	{
		m_queues.emplace(queue_pointer);
	}
}

} // namespace hardware
} // namespace xmipp4
