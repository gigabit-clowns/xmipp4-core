// SPDX-License-Identifier: GPL-3.0-only

#include "caching_memory_allocator.hpp"

#include "caching_buffer_sentinel.hpp"

#include <xmipp4/core/logger.hpp>
#include <xmipp4/core/hardware/memory_heap.hpp>

namespace xmipp4
{
namespace hardware
{

static
void wait_on_queues(span<device_queue *const> queues)
{
	for (const device_queue *queue : queues)
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

	// Align the size to a multiple of the maximum alignment so that in case of
	// partitioning the block, the next block remains maximally aligned.
	size = memory::align_ceil(size, m_maximum_alignment);

	m_deferred_release.process_pending_free(m_pool);
	auto *block = m_pool.find_suitable_block(size, queue);

	if (!block)
	{
		// No suitable block was found in the pool. Request more memory.
		const auto request_size = memory::align_ceil(size, m_heap_size_step);
		std::shared_ptr<memory_heap> heap;
		try
		{
			heap = get_memory_resource().create_memory_heap(
				request_size, 
				m_maximum_alignment
			);
		}
		catch (const std::bad_alloc&)
		{
			XMIPP4_LOG_WARN(
				"Memory allocation failed. Retrying after releasing resources."
			);
			m_deferred_release.wait_pending_free(m_pool);
			m_pool.release_unused_heaps();
			heap = get_memory_resource().create_memory_heap(
				request_size,
				m_maximum_alignment
			);
		}

		XMIPP4_ASSERT( heap );
		block = m_pool.register_heap(std::move(heap), queue);
	}

	XMIPP4_ASSERT( block );
	const auto block_size = block->get_size();
	XMIPP4_ASSERT( block_size >= size );
	const auto remaining = block_size - size;
	if (remaining >= m_maximum_alignment)
	{
		// Only partition if the block has a reminder larger than the 
		// step size.
		std::tie(block, std::ignore) = m_pool.partition_block(
			block,
			size,
			remaining
		); 
	}

	XMIPP4_ASSERT( block );
	return create_buffer(*block);
}

void caching_memory_allocator::recycle_block(
	memory_block &block, 
	span<device_queue *const> queues
)
{
	if (queues.empty())
	{
		m_pool.release(block);
	}
	else if (m_device)
	{
		m_deferred_release.defer_release(block, queues, *m_device);
	}
	else
	{
		wait_on_queues(queues);
	}
}

std::shared_ptr<buffer> 
caching_memory_allocator::create_buffer(memory_block &block)
{
	auto *heap = block.get_heap();
	const auto offset = block.get_offset();
	const auto size = block.get_size();

	auto sentinel = 
			std::make_unique<caching_buffer_sentinel>(*this, block);
	m_pool.acquire(block);

	XMIPP4_ASSERT(heap);
	return heap->create_buffer(offset, size, std::move(sentinel));
}

} // namespace hardware
} // namespace xmipp4
