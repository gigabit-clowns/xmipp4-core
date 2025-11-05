// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>
#include <memory>

#include "memory_block.hpp"
#include "memory_block_context.hpp"

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
    using iterator = memory_block_context::iterator;

    /**
     * @brief Obtain an iterator to the begining.
     * 
     * @return iterator The iterator to the begining.
     */
    iterator begin();

    /**
     * @brief Obtain an iterator to the end.
     * 
     * @return iterator The iterator to the end.
     */
    iterator end();

    /**
     * @brief Find a candidate block.
     * 
     * If found, the returned block will be at least of the requested size
     * and with the requested block id.
     * 
     * @param blocks Collection of blocks.
     * @param size Minimum size of the block.
     * @param alignment Minimum alignment of the block.
     * @param queue Queue of the requested block.
     * @return iterator Iterator the candidate block. end() if none was found.
     * 
     */
    iterator find_suitable_block(
        std::size_t size,
        std::size_t alignment, 
        const device_queue *queue 
    );

    /**
     * @brief Partition a block in two blocks.
     * 
     * @param ite Iterator to the block to be partitioned. Must be dereferenceable.
     * @param size Size of the first partition.
     * @param remaining Size of the second partition.
     * @return std::pair<iterator, iterator> Iterators to the resulting 
     * two partitions.
     * 
     */
    std::pair<iterator, iterator> partition_block(
        iterator ite,
        std::size_t size,
        std::size_t remaining
    );

    /**
     * @brief Register a new memory heap to serve future allocations.
     * 
     * @param heap The heap to be registered. Can not be nullptr.
     * @param queue The queue served by this heap.
     * @return iterator Position where it was inserted.
     */
    iterator register_heap(
        std::shared_ptr<memory_heap> heap, 
        const device_queue *queue
    );

    /**
     * @brief Return a block to the pool.
     * 
     * This marks the provided block as free. If possible, it merges it with the
     * neighboring blocks.
     * 
     * @param ite Iterator to the block to be returned. 
     * It must belong to the provided pool.
     * 
     */
    void recycle_block(iterator block_iterator);

    /**
     * @brief Release free blocks when possible.
     * 
     * All free blocks that are not partitioned are returned to the allocator.
     * 
     */
    void release_blocks();

private:
    std::map<memory_block, memory_block_context, memory_block_less> m_blocks;

    /**
     * @brief Merge adjacent blocks if necessary.
     * 
     * Forward and/or backward blocks are merged if they are free.
     * 
     * @param ite Iterator to the candidate block. Must be dereferenceable.
     * @return iterator Input iterator if not merged. Iterator to the merged 
     * block when merged.
     * 
     */
    iterator consider_merging_block(iterator ite);

    /**
     * @brief Merge two blocks.
     * 
     * @param first Iterator to the first block to be merged. 
     * Must be dereferenceable.
     * @param second Iterator to the second block to be merged. 
     * Must be dereferenceable.
     * @return iterator Iterator to the merged blocks.
     * 
     */
    iterator merge_blocks(
        iterator first,
        iterator second
    );

    /**
     * @brief Merge three blocks.
     * 
     * @param first Iterator to the first block to be merged. 
     * Must be dereferenceable.
     * @param second Iterator to the second block to be merged. 
     * Must be dereferenceable.
     * @param third Iterator to the third block to be merged. 
     * Must be dereferenceable.
     * @return iterator Iterator to the merged blocks.
     * 
     */
    iterator merge_blocks(
        iterator first,
        iterator second,
        iterator third
    );

};

} // namespace hardware
} // namespace xmipp4
