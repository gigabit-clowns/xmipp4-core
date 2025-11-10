// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <map>

namespace xmipp4 
{
namespace hardware
{

class memory_block;
class memory_block_less;

/**
 * @brief Control block to manage the context of memory blocks in
 * a pool.
 * 
 */
class memory_block_context
{
public:
    using iterator = std::map<
        memory_block, 
        memory_block_context, 
        memory_block_less
    >::iterator;

    /**
     * @brief Construct a new cuda memory block context
     * 
     * @param prev Iterator to the previous partition of a common allocation. 
     * May be null.
     * @param next Iterator to the next partition of a common allocation. 
     * May be null.
     * @param free Wether the block is available.
     * 
     */
    memory_block_context(
        iterator prev, 
        iterator next, 
        bool free
    ) noexcept;
    memory_block_context(const memory_block_context &other) = default;
    memory_block_context(memory_block_context &&other) = default;
    ~memory_block_context() = default;

    memory_block_context& 
    operator=(const memory_block_context &other) = default;
    memory_block_context& 
    operator=(memory_block_context &&other) = default;

    /**
     * @brief Set the previous partition.
     * 
     * @param prev Iterator to the previous partition of a common allocation. 
     * May be null.
     * 
     */
    void set_previous_block(iterator prev) noexcept;

    /**
     * @brief Get the previous partition.
     * 
     * @return iterator Iterator to the previous partition of a common 
     * allocation. May be null.
     * 
     */
    iterator get_previous_block() const noexcept;

    /**
     * @brief Set the next partition.
     * 
     * @param next Iterator to the next partition of a common allocation. 
     * May be null.
     * 
     */
    void set_next_block(iterator next) noexcept;

    /**
     * @brief Get the next partition.
     * 
     * @return iterator Iterator to the next partition of a common 
     * allocation. May be null.
     * 
     */
    iterator get_next_block() const noexcept;

    /**
     * @brief Set wether the block is available.
     * 
     * @param free true if it is available, false otherwise.
     */
    void set_free(bool free);

    /**
     * @brief Check wether the block is available.
     * 
     * @return true Block is free.
     * @return false Block is occupied.
     */
    bool is_free() const noexcept;

private:
    iterator m_prev;
    iterator m_next;
    bool m_free;

}; 



bool operator==(
    const memory_block_context &lhs,
    const memory_block_context &rhs
) noexcept;

bool operator!=(
    const memory_block_context &lhs,
    const memory_block_context &rhs
) noexcept;



/**
 * @brief Check if a block is partitioned.
 * 
 * A block is considered to be partitioned if it has a previous or next
 * partition iterator set to a non-empty value.
 * 
 * @param block Block to be checked.
 * @return true Block is partitioned.
 * @return false Block is not partitioned.
 * 
 */
bool is_partition(const memory_block_context &block) noexcept;

} // namespace hardware
} // namespace xmipp4

#include "memory_block_context.inl"
