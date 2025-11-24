// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <cstddef>
#include <memory>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/set.hpp>

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class memory_heap;

/**
 * @brief Represents a chunk of data managed by a caching allocator. 
 * 
 */
class memory_block
{
public:
	using block_list_hook_type =
		boost::intrusive::list_member_hook<>;
	using free_block_set_hook_type =
		boost::intrusive::set_member_hook<>;
	
	block_list_hook_type block_list_hook;
	free_block_set_hook_type free_block_set_hook;

	/**
	 * @brief Construct a new cuda memory block from its components.
	 * 
	 * @param queue Queue where this belongs.
	 * @param size Number of bytes referenced.
	 * @param heap Non owning pointer to the heap associated to this block.
	 * @param offset Offset of this block into the heap.
	 * 
	 */
	memory_block(
		const device_queue *queue,
		std::size_t size,
		memory_heap *heap,
		std::size_t offset
	) noexcept;

	memory_block(const memory_block &other) = default;
	memory_block(memory_block &&other) = default;
	~memory_block() = default;

	memory_block& operator=(const memory_block &other) = default;
	memory_block& operator=(memory_block &&other) = default;

	/**
	 * @brief Get the queue where this block is synchronous.
	 * 
	 * @return const device_queue* Pointer to the queue.
	 */
	const device_queue* get_queue() const noexcept;

	/**
	 * @brief Set the size of the block.
	 * 
	 * @param size The new size.
	 */
	void set_size(std::size_t size) noexcept;

	/**
	 * @brief Get the number of bytes referenced by this object.
	 * 
	 * @return std::size_t Number of bytes.
	 */
	std::size_t get_size() const noexcept;

	/**
	 * @brief Get a pointer to the heap
	 * 
	 * @return memory_heap* Non-owning pointer to the heap associated 
	 * to this block.
	 */
	memory_heap* get_heap() const noexcept;

	/**
	 * @brief Set the offset into the heap.
	 * 
	 */
	void set_offset(std::size_t offset) noexcept;

	/**
	 * @brief Get the offset of the memory block within the heap.
	 * 
	 * @return std::size_t The offset.
	 */
	std::size_t get_offset() const noexcept;

	/**
	 * @brief Check wether the block is free or not. 
	 * 
	 * @return true The block is free.
	 * @return false The block is occupied.
	 */
	bool is_free() const noexcept;

private:
	const device_queue *m_queue;
	std::size_t m_size;
	memory_heap *m_heap;
	std::size_t m_offset;

}; 

bool operator==(const memory_block &lhs, const memory_block &rhs) noexcept;
bool operator!=(const memory_block &lhs, const memory_block &rhs) noexcept;

} // namespace hardware
} // namespace xmipp4

#include "memory_block.inl"
