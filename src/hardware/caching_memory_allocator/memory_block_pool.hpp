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
     * and with the requested device_queue.
     * 
     * @param blocks Collection of blocks.
     * @param size Minimum size of the block.
     * @param queue Queue of the requested block.
     * @return iterator Iterator the candidate block. end() if none was found.
     * 
     */
    iterator find_suitable_block(
        std::size_t size,
        const device_queue *queue 
    );

    /**
     * @brief Partition a block in two blocks.
     * 
     * @param ite Iterator to the block to be partitioned. Must be dereferenceable.
     * @param size1 Size of the first partition.
     * @param size2 Size of the second partition.
     * @return std::pair<iterator, iterator> Iterators to the resulting 
     * two partitions.
     * 
     */
    std::pair<iterator, iterator> partition_block(
        iterator ite,
        std::size_t size1,
        std::size_t size2
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
     * @brief Release free blocks when possible.
     * 
     * All free blocks that are not partitioned are returned to the allocator.
     * 
     */
    void release_blocks();

private:
    std::map<memory_block, memory_block_context, memory_block_less> m_blocks;

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

    /**
     * @brief Update links for the next partition to ensure consistency.
     * 
     * If the provided element has a next partition, the previous partition
     * is updated on the next partition. Otherwise does nothing.
     * 
     * @param ite Iterator to a block. Must be dereferenceable.
     * 
     */
    void update_forward_link(iterator ite) noexcept;

    /**
     * @brief Update links for the previous partition to ensure consistency.
     * 
     * If the provided element has a previous partition, the next partition
     * is updated on the previous partition. Otherwise does nothing.
     * 
     * @param ite Iterator to a block. Must be dereferenceable.
     * 
     */
    void update_backward_link(iterator ite) noexcept;

    /**
     * @brief Update links for the previous and next partitions to ensure 
     * consistency.
     * 
     * @param ite Iterator to a block. Must be dereferenceable.
     * 
     * @see update_forward_link
     * @see update_backward_link
     * 
     */
    void update_links(iterator ite) noexcept;

    /**
     * @brief Check for consistency in the next partition.
     * 
     * The forward link is consistent if the previous block of next block of 
     * the iterator is the iterator itself. If the next block is end(), it 
     * is considered to be consistent.
     * 
     * @param ite Iterator to a block. Must be dereferenceable.
     * @return true Forward link is consistent.
     * @return false Forward link is not consistent. Thus, an error occurred.
     * 
     */
    bool check_forward_link(iterator ite)  noexcept;

    /**
     * @brief Check for consistency in the previous partition.
     * 
     * The backward link is consistent if the next block of the previous block 
     * of the iterator is the iterator itself. If the previous block is end(), 
     * it is considered to be consistent.
     * 
     * @param ite Iterator to a block. Must be dereferenceable.
     * @return true Backward link is consistent.
     * @return false Backward link is not consistent. Thus, an error ocurred.
     * 
     */
    bool check_backward_link(iterator ite) noexcept;

    /**
     * @brief Check for consistency in the previous and next partitions.
     * 
     * @param ite Iterator to a block. Must be dereferenceable.
     * @return true Forward and backward links are consistent.
     * @return false Forward and backward links are not consistent. 
     * Thus, an error ocurred.
     * 
     */
    bool check_links(iterator ite) noexcept;

    /**
     * @brief Check if a block is partitioned.
     * 
     * A block is considered to be partitioned if it has a previous or next
     * partition iterator set to a non-empty value.
     * 
     * @param ite Iterator to the block to be checked. Must be dereferenceable.
     * @return true Block is partitioned.
     * @return false Block is not partitioned.
     * 
     */
    bool is_partition(iterator ite) noexcept;

    /**
     * @brief Check if a block can be merged to.
     * 
     * A block can be merged if it is free.
     * 
     * @param ite Iterator to the block to be checked. May be end().
     * @return true Item pointed by the iterator is free.
     * @return false Item pointed by the iterator is occupied or the 
     * provided iterator is null.
     * 
     */
    bool is_mergeable(iterator ite) noexcept;

};

} // namespace hardware
} // namespace xmipp4
