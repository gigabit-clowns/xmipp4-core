// SPDX-License-Identifier: GPL-3.0-only

#include "memory_block_pool.hpp"

#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/hardware/memory_heap.hpp>
#include <xmipp4/core/platform/assert.hpp>

#include <tuple>

namespace xmipp4
{
namespace hardware
{

bool memory_block_pool::free_memory_block_compare::operator()(
	const memory_block &lhs, 
	const memory_block &rhs
) const noexcept
{
	return std::make_tuple(lhs.get_queue(), lhs.get_size()) <
	       std::make_tuple(rhs.get_queue(), rhs.get_size()) ;
}



void memory_block_pool::acquire(memory_block &block) noexcept
{
	auto ite = m_free_blocks.iterator_to(block);
	XMIPP4_ASSERT( ite != m_free_blocks.end() );
	m_free_blocks.erase(ite);
}

void memory_block_pool::release(memory_block &block) noexcept
{
	consider_merging_block(block);
	m_free_blocks.insert(block);
}

memory_block* memory_block_pool::find_suitable_block(
	std::size_t size,
	const device_queue *queue 
)
{
	// Assuming that the blocks are ordered according to their queue reference
	// first and then their sizes, the best fit is achieved for the first block
	// that has at least the requested size and the requested queue.
	const memory_block key(queue, size, nullptr, 0UL);
	auto ite = m_free_blocks.lower_bound(key);

	if (ite == m_free_blocks.end())
	{
		return nullptr;
	}
	if(ite->get_queue() != queue)
	{
		return nullptr;
	}

	XMIPP4_ASSERT(ite->get_size() >= size);
	return &(*ite);
}

std::pair<memory_block*, memory_block*>
memory_block_pool::partition_block(
	memory_block* block,
	std::size_t size1,
	std::size_t size2
)
{
	XMIPP4_ASSERT( block );
	XMIPP4_ASSERT( is_free(*block) );

	auto *first = new memory_block(
		block->get_queue(),
		size1,
		block->get_heap(),
		block->get_offset()
	);
	
	m_free_blocks.erase(m_free_blocks.iterator_to(*block));
	auto *second = block;
	second->set_size(size2);
	second->set_offset(size1 + second->get_offset());

	// Insert first before second (already in the list)
	m_blocks.insert(m_blocks.iterator_to(*second), *first);

	m_free_blocks.insert(*first);
	m_free_blocks.insert(*second);

	return std::make_pair(first, second);
}

memory_block* memory_block_pool::register_heap(
	std::shared_ptr<memory_heap> heap, 
	const device_queue *queue
)
{
	auto block = std::make_unique<memory_block>(
		queue, 
		heap->get_size(), 
		heap.get(), 
		0
	);
	
	bool inserted;
	std::tie(std::ignore, inserted) = m_heaps.emplace(std::move(heap));
	XMIPP4_ASSERT( inserted );

	m_blocks.push_back(*block);
	m_free_blocks.insert(*block);

	return block.release();
}

void memory_block_pool::consider_merging_block(memory_block &block) noexcept
{
	consider_merging_forwards(block);
	consider_merging_backwards(block);
}

void memory_block_pool::release_unused_heaps()
{
	auto ite = m_free_blocks.begin();
	while (ite != m_free_blocks.end())
	{
		if (!is_partition(*ite))
		{
			auto *block = &(*ite);
			auto *heap = block->get_heap();
			XMIPP4_ASSERT(heap);

			ite = m_free_blocks.erase(ite);
			m_blocks.erase(m_blocks.iterator_to(*block));
			release(*heap);

			delete block;
		}
		else
		{
			++ite;
		}
	}
}

bool memory_block_pool::is_free(const memory_block &block) noexcept
{
	return block.free_block_set_hook.is_linked();
}

void memory_block_pool::consider_merging_forwards(memory_block &block) noexcept
{
	const auto ite = m_blocks.iterator_to(block);
	const auto next = std::next(ite);
	if (next == m_blocks.end() || 
		next->get_heap() != ite->get_heap() ||
		!is_free(*next)
	)
	{
		return;
	}

	auto *next_block = &(*next);
	m_blocks.erase(next);
	m_free_blocks.erase(m_free_blocks.iterator_to(*next_block));
	
	block.set_size(block.get_size() + next_block->get_size());

	delete next_block;	
}

void memory_block_pool::consider_merging_backwards(memory_block &block) noexcept
{
	const auto ite = m_blocks.iterator_to(block);
	if (ite == m_blocks.begin())
	{
		return;
	}

	auto prev = std::prev(ite);
	if (prev->get_heap() != ite->get_heap() || !is_free(*prev))
	{
		return;
	}

	auto *prev_block = &(*prev);
	m_blocks.erase(prev);
	m_free_blocks.erase(m_free_blocks.iterator_to(*prev));
	
	block.set_size(block.get_size() + prev_block->get_size());
	block.set_offset(prev_block->get_offset());

	delete prev_block;	
}

bool memory_block_pool::is_partition(const memory_block &block) const noexcept
{
	XMIPP4_ASSERT(block.block_list_hook.is_linked());

	const auto *heap = block.get_heap();
	const auto ite = m_blocks.iterator_to(block);

	if (ite != m_blocks.cbegin() && std::prev(ite)->get_heap() == heap)
	{
		return true;
	}

	const auto next = std::next(ite);
	if (next != m_blocks.cend() && next->get_heap() == heap)
	{
		return true;
	}

	return false;
}

void memory_block_pool::release(const memory_heap &heap)
{
	const auto ite = m_heaps.find(
		&heap,
		std::hash<const memory_heap*>(),
		[] (const memory_heap *lhs, const std::shared_ptr<memory_heap> &rhs)
		{
			return rhs.get() == lhs;
		}
	);
	XMIPP4_ASSERT( ite != m_heaps.end() );
	m_heaps.erase(ite);
}

} // namespace hardware
} // namespace xmipp4
