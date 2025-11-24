// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "memory_block.hpp"

#include <cstddef>
#include <memory>
#include <set>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>
#include <boost/unordered/unordered_set.hpp>

namespace xmipp4 
{
namespace hardware
{

/**
 * @brief Keeps track of a set of memory_block-s that can belong to multiple
 * device_queue-s and memory_heap-s
 * 
 */
class memory_block_pool
{
public:
	memory_block_pool() = default;
	memory_block_pool(const memory_block_pool &other) = delete;
	memory_block_pool(memory_block_pool &&other) = delete;
	~memory_block_pool();

	memory_block_pool& operator=(const memory_block_pool &other) = delete;
	memory_block_pool& operator=(memory_block_pool &&other) = delete;

	/**
	 * @brief Mark the block pointed by the block as occupied.
	 * 
	 * @param block Reference to the block to be marked as occupied.
	 * The block must be free.
	 */
	void acquire(memory_block &block) noexcept;

	/**
	 * @brief Mark the block pointed by the block as free.
	 * 
	 * @param block Reference to the block to be marked as free. The block
	 * must be occupied.
	 */
	void release(memory_block &block) noexcept;

	/**
	 * @brief Find a candidate block.
	 * 
	 * If found, the returned block will be at least of the requested size
	 * and with the requested device_queue.
	 * 
	 * @param size Minimum size of the block.
	 * @param queue Queue of the requested block.
	 * @return memory_block* Pointer the candidate block. nullptr if none 
	 * was found.
	 * 
	 */
	memory_block* find_suitable_block(
		std::size_t size,
		const device_queue *queue 
	);

	/**
	 * @brief Partition a block in two blocks.
	 * 
	 * @param block Pointer to the block to be partitioned. Must be dereferenceable.
	 * @param size1 Size of the first partition.
	 * @param size2 Size of the second partition.
	 * @return std::pair<memory_block*, memory_block*> Pointers to the resulting 
	 * two partitions.
	 * 
	 */
	std::pair<memory_block*, memory_block*> partition_block(
		memory_block *block,
		std::size_t size1,
		std::size_t size2
	);

	/**
	 * @brief Register a new memory heap to serve future allocations.
	 * 
	 * @param heap The heap to be registered. Can not be nullptr.
	 * @param queue The queue served by this heap.
	 * @return memory_block* Pointer to the inserted block. Guaranteed to not
	 * be nullptr.
	 */
	memory_block* register_heap(
		std::shared_ptr<memory_heap> heap, 
		const device_queue *queue
	);

	/**
	 * @brief Merge adjacent blocks if necessary.
	 * 
	 * Forward and/or backward blocks are merged if they are free.
	 * 
	 * @param block Reference to the candidate block.
	 * 
	 */
	void consider_merging_block(memory_block &block) noexcept;

	/**
	 * @brief Release free blocks when possible.
	 * 
	 * All free blocks that are not partitioned are returned to the allocator.
	 * 
	 */
	void release_unused_heaps();

private:
	class free_memory_block_compare
	{
	public:
		bool operator()(
			const memory_block &lhs, 
			const memory_block &rhs
		) const noexcept;

	};



	using memory_block_list_type = boost::intrusive::list<
		memory_block,
    	boost::intrusive::member_hook<
			memory_block, 
			memory_block::block_list_hook_type, 
			&memory_block::block_list_hook
		>
	>;
	using free_memory_block_set_type = boost::intrusive::multiset<
		memory_block,
    	boost::intrusive::member_hook<
			memory_block, 
			memory_block::free_block_set_hook_type, 
			&memory_block::free_block_set_hook
		>,
		boost::intrusive::compare<free_memory_block_compare>
	>;
	using heap_set_type = boost::unordered::unordered_set<
		std::shared_ptr<memory_heap>,
		std::hash<std::shared_ptr<memory_heap>>
	>;

	memory_block_list_type m_blocks;
	free_memory_block_set_type m_free_blocks;
	heap_set_type m_heaps;

	/**
	 * @brief Consider merging the provided block with the next one.
	 * 
	 * The block is merged if it has a "next" block, this belongs to the
	 * same allocation and it is free.
	 * 
	 * @param block The block to be merged forwards.
	 */
	void consider_merging_forwards(memory_block &block) noexcept;

	/**
	 * @brief Consider merging the provided block with the previous one.
	 * 
	 * The block is merged if it has a "previous" block, this belongs to the
	 * same allocation and it is free.
	 * 
	 * @param block The block to be merged backwards.
	 */
	void consider_merging_backwards(memory_block &block) noexcept;

	/**
	 * @brief Check if a block is partitioned.
	 * 
	 * A block is considered to be partitioned if the next or previous
	 * blocks refer to the same heap.
	 * @return true Block is partitioned.
	 * @return false Block is not partitioned.
	 * 
	 */
	bool is_partition(const memory_block &block) const noexcept;

	/**
	 * @brief Release the provided memory heap.
	 * 
	 * @param heap The memory heap to be released.
	 */
	void release(const memory_heap &heap);

};

} // namespace hardware
} // namespace xmipp4
