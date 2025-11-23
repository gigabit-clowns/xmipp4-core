// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>
#include <xmipp4/core/hardware/device.hpp>
#include <xmipp4/core/hardware/device_to_host_event.hpp>
#include <xmipp4/core/hardware/device_queue.hpp>

#include "memory_block.hpp"
#include "memory_block_pool.hpp"

#include <forward_list>
#include <utility>
#include <vector>

namespace xmipp4 
{
namespace hardware
{

class memory_block_pool;

/**
 * @brief Handles deferred deallocations to allow mixing multiple
 * device_queues.
 * 
 */
class memory_block_deferred_release
{
public:
	memory_block_deferred_release() = default;
	memory_block_deferred_release(
			const memory_block_deferred_release &other
	) = delete;
	memory_block_deferred_release(
			memory_block_deferred_release &&other
	) = default;
	~memory_block_deferred_release() = default;

	memory_block_deferred_release&
	operator=(const memory_block_deferred_release &other) = delete;
	memory_block_deferred_release&
	operator=(memory_block_deferred_release &&other) = default;

	/**
	 * @brief Wait for all pending free-s to complete.
	 * 
	 * @param pool The pool from which all blocks were obtained.
	 * 
	 */
	void wait_pending_free(memory_block_pool &pool);

	/**
	 * @brief Iterate through release events and return back all blocks
	 * that have no pending events.
	 * 
	 * @param pool The pool from which all blocks were obtained.
	 * 
	 */
	void process_pending_free(memory_block_pool &pool);

	/**
	 * @brief Defer the release of a memory block. 
	 * 
	 * @param block The memory block to be released.
	 * @param other_queues Queues that need to be processed for actually
	 * freeing the block.
	 * @param device The device associated to the queues. Must be the
	 * same across calls.
	 * 
	 * @note This function does not check wether ite has been provided 
	 * previously. Calling it twice with the same block before it has
	 * been returned to the pool leads to undefined behavior.
	 * 
	 */
	void defer_release(
		memory_block_pool::iterator block, 
		span<device_queue *const> other_queues,
		device &device
	);

private:
	using event_list = std::forward_list<std::shared_ptr<device_to_host_event>>;

	event_list m_event_pool;
	std::vector<std::pair<memory_block_pool::iterator, event_list>> m_pending_free;

	/**
	 * @brief Pop all signaled events from the list.
	 * 
	 * @param events Event list from which completed events are popt.
	 */
	void pop_completed_events(event_list &events);

	/**
	 * @brief Record an event for a queue in the list.
	 * 
	 */
	void record_event(event_list &events, device_queue &queue, device &device);
}; 

} // namespace hardware
} // namespace xmipp4
