// SPDX-License-Identifier: GPL-3.0-only

#include "memory_block_deferred_release.hpp"

#include "memory_block_pool.hpp"

#include <algorithm>

namespace xmipp4
{
namespace hardware
{

void memory_block_deferred_release::wait_pending_free(
	memory_block_pool &pool
)
{
	for (auto &item : m_pending_free)
	{
		auto &events = item.second;
		for (const auto &event : item.second)
		{
			XMIPP4_ASSERT(event);
			event->wait();
		}

		auto block = item.first;
		pool.release(block);
		m_event_pool.splice_after(m_event_pool.cbefore_begin(), events);
	}

	m_pending_free.clear();
}

void memory_block_deferred_release::process_pending_free(
	memory_block_pool &pool
)
{
	const auto last = std::remove_if(
		m_pending_free.begin(), m_pending_free.end(),
		[this, &pool] (auto &item) -> bool
		{
			// Remove all completed events
			auto &events = item.second;
			pop_completed_events(events);

			// Return block if completed
			const auto remove = events.empty();
			if(remove)
			{
				auto block = item.first;
				block->second.set_free(true);
				pool.consider_merging_block(block);
			}

			return remove;
		}
	);

	m_pending_free.erase(last, m_pending_free.end());
}

void memory_block_deferred_release::defer_release(
	memory_block_pool::iterator block, 
	span<device_queue *const> queues,
	device &device
)
{
	if (queues.empty())
	{
		throw std::invalid_argument(
			"No queues were provided to defer the release"
		);
	}

	// Add a new entry to the pending free list.
	m_pending_free.emplace_back(
		std::piecewise_construct,
		std::forward_as_tuple(block),
		std::forward_as_tuple()
	);

	// Record an event for each of the queues.
	auto& events = m_pending_free.back().second;
	for (device_queue *queue : queues)
	{
		if (!queue)
		{
			throw std::invalid_argument("nullptr queue was provided");
		}

		record_event(events, *queue, device);
	}
}

void memory_block_deferred_release::pop_completed_events(
		event_list &events
)
{
	auto prev_ite = events.cbefore_begin();
	event_list::const_iterator ite;
	while ((ite = std::next(prev_ite)) != events.cend())
	{
		const auto &event = *ite;
		XMIPP4_ASSERT( event ); 
		if(event->is_signaled())
		{
			// Return the event to the pool
			m_event_pool.splice_after(
				m_event_pool.cbefore_begin(),
				events,
				prev_ite
			);
		}
		else
		{
			++prev_ite;
		}
	}
}

void memory_block_deferred_release::record_event(
	event_list &events, 
	device_queue &queue, 
	device &device
)
{
	// Add a new event to the front
	if (m_event_pool.empty())
	{
		// No events in the pool.
		events.emplace_front(device.create_device_to_host_event());
	}
	else
	{
		// Re-use an event from the pool.
		events.splice_after(
			events.cbefore_begin(),
			m_event_pool, 
			m_event_pool.cbefore_begin()
		);
	}

	XMIPP4_ASSERT( events.front() );
	events.front()->signal(queue);
}

} // namespace hardware
} // namespace xmipp4
