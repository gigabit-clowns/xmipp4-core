// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/span.hpp>

#include <cstddef>
#include <memory>

namespace xmipp4 
{
namespace hardware
{

class device_queue;
class memory_heap;

/**
 * @brief Represents a chunk of data managed by a caching allocator.
 * 
 * It contains an unique id to a queue where this data is synchronous.
 * It also contains the size of the referenced block, an owning memory heap
 * and the offset of the data within that heap
 * 
 */
class memory_block
{
public:
	/**
	 * @brief Construct a new cuda memory block from its components.
	 * 
	 * @param data_ptr Pointer to the data.
	 * @param size Number of bytes referenced.
	 * @param queue Queue where this belongs.
	 */
	memory_block(
		std::shared_ptr<memory_heap> heap,
		std::size_t offset,
		std::size_t size, 
		const device_queue *queue 
	) noexcept;

	memory_block(const memory_block &other) = default;
	memory_block(memory_block &&other) = default;
	~memory_block() = default;

	memory_block& operator=(const memory_block &other) = default;
	memory_block& operator=(memory_block &&other) = default;

	/**
	 * @brief Get a pointer to the heap
	 * 
	 * @return memory_heap* Non-owning pointer to the heap.
	 */
	memory_heap* get_heap() const noexcept;

	/**
	 * @brief Obtain a shared copy of the heap resource
	 * 
	 */
	std::shared_ptr<memory_heap> share_heap() const noexcept;

	/**
	 * @brief Get the offset of the memory block within the heap.
	 * 
	 * @return std::size_t The offset.
	 */
	std::size_t get_offset() const noexcept;

	/**
	 * @brief Get the number of bytes referenced by this object.
	 * 
	 * @return std::size_t Number of bytes.
	 */
	std::size_t get_size() const noexcept;

	/**
	 * @brief Get the queue where this block is synchronous.
	 * 
	 * @return const device_queue* Pointer to the queue.
	 */
	const device_queue* get_queue() const noexcept;

private:
	const device_queue *m_queue;
	std::size_t m_size;
	std::shared_ptr<memory_heap> m_heap;
	std::size_t m_offset;
}; 

bool operator==(const memory_block &lhs, const memory_block &rhs) noexcept;
bool operator!=(const memory_block &lhs, const memory_block &rhs) noexcept;

/**
 * @brief Lexicographically compare two memory_block objects.
 * 
 * First, queue IDs are compared.
 * If equal, then, sizes are compared.
 * If equal, then the heap pointers are compared.
 * If equal, then the offsets of memory pointers are compared.
 * 
 */
class memory_block_less
{
public:
	bool operator()(
		const memory_block &lhs, 
		const memory_block &rhs
	) const noexcept;

private:
	using tuple_type = std::tuple<
		const device_queue*,
		std::size_t,
		memory_heap*,
		std::size_t
	>;

	static
	tuple_type as_tuple(const memory_block &block) noexcept;
};

} // namespace hardware
} // namespace xmipp4

#include "memory_block.inl"
