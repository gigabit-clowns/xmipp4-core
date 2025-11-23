// SPDX-License-Identifier: GPL-3.0-only

#include "memory_block_pool.hpp"

#include <xmipp4/core/memory/align.hpp>
#include <xmipp4/core/hardware/memory_heap.hpp>

namespace xmipp4
{
namespace hardware
{

memory_block_pool::iterator memory_block_pool::begin()
{
	return m_blocks.begin();
}

memory_block_pool::iterator memory_block_pool::end()
{
	return m_blocks.end();
}


void memory_block_pool::acquire(iterator ite)
{
	ite->second.set_free(false);
}

void memory_block_pool::release(iterator ite)
{
	ite->second.set_free(true);
	consider_merging_block(ite);
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
	for (auto ite = m_blocks.lower_bound(key); ite != m_blocks.end(); ++ite)
	{
		if(ite->first.get_queue() != queue)
		{
			// Reached the end of the allowed range.
			break;
		}
		
		if (ite->second.is_free())
		{
			// Found a suitable block.
			return ite;
		}
	}

	return m_blocks.end();
}

std::pair<memory_block_pool::iterator, memory_block_pool::iterator>
memory_block_pool::partition_block(
	iterator ite,
	std::size_t size1,
	std::size_t size2
)
{
	auto heap = ite->first.share_heap();
	const auto base_offset = ite->first.get_offset();
	const auto queue = ite->first.get_queue();
	const auto prev = ite->second.get_previous_block();
	const auto next = ite->second.get_next_block();
	const auto is_free = ite->second.is_free();

	iterator first;
	iterator second;
	bool inserted;
	std::tie(first, inserted) = m_blocks.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(
			heap,
			base_offset, 
			size1, 
			queue
		),
		std::forward_as_tuple(
			prev,
			memory_block_pool::iterator(), // To be set
			is_free
		)
	);
	XMIPP4_ASSERT(inserted);
	std::tie(second, inserted) = m_blocks.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(
			std::move(heap), // No longer needed
			base_offset + size1, 
			size2, 
			queue
		),
		std::forward_as_tuple(
			first,
			next,
			is_free
		)
	);
	XMIPP4_ASSERT(inserted);
	
	first->second.set_next_block(second);
	update_backward_link(first);
	update_forward_link(second);

	XMIPP4_ASSERT( check_links(first) );
	XMIPP4_ASSERT( check_links(second) );

	// Remove old block
	m_blocks.erase(ite);

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

	std::tie(result, inserted) = m_blocks.emplace(
		std::piecewise_construct,
		std::forward_as_tuple(std::move(heap), 0UL, heap->get_size(), queue),
		std::forward_as_tuple(end(), end(), true)
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
	XMIPP4_ASSERT( ite->second.is_free() );
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
		if(ite->second.is_free() && !is_partition(ite))
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

	auto heap = first->first.share_heap();
	const auto offset = first->first.get_offset();
	const auto size = first->first.get_size() + second->first.get_size() ;
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
	const auto size = 
		first->first.get_size() +
		second->first.get_size() +
		third->first.get_size();
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

void memory_block_pool::update_forward_link(iterator ite) noexcept
{
	const auto next = ite->second.get_next_block();
	if (next != end())
	{
		next->second.set_previous_block(ite);
	}
}

void memory_block_pool::update_backward_link(iterator ite) noexcept
{
	const auto prev = ite->second.get_previous_block();
	if (prev != end())
	{
		prev->second.set_next_block(ite);
	}
}

void memory_block_pool::update_links(iterator ite) noexcept
{
	update_backward_link(ite);
	update_forward_link(ite);
}

bool memory_block_pool::check_forward_link(iterator ite) noexcept
{
	const auto next = ite->second.get_next_block();
	if (next == end())
	{
		return true;
	}

	return next->second.get_previous_block() == ite;
}

bool memory_block_pool::check_backward_link(iterator ite) noexcept
{
	const auto prev = ite->second.get_previous_block();
	if (prev == end())
	{
		return true;
	}
	
	return prev->second.get_next_block() == ite;
}

bool memory_block_pool::check_links(iterator ite) noexcept
{
	return check_backward_link(ite) && check_forward_link(ite);
}

bool memory_block_pool::is_partition(iterator ite) noexcept
{
	const auto &context = ite->second;
	const auto prev = context.get_previous_block();
	const auto next = context.get_next_block();
	return next != end() || prev != end();
}

bool memory_block_pool::is_mergeable(iterator ite) noexcept
{
	if (ite == end())
	{
		return false;
	}
	
	return ite->second.is_free();
}

} // namespace hardware
} // namespace xmipp4
