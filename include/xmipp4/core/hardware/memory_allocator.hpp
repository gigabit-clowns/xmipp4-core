// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <memory>

#include <xmipp4/core/platform/dynamic_shared_object.h>

namespace xmipp4
{
namespace hardware
{

class buffer;
class device_queue;
class memory_resource;

/**
 * @brief Abstract allocator of @ref buffer objects on a @ref memory_resource.
 *
 * Concrete implementations are free to back themselves with anything that fits 
 * the underlying backend.
 *
 * The allocator is also in charge of tracking the asynchronous use of the 
 * buffers it produces: when a buffer's shared ownership is dropped while device
 * work referencing it is still in flight, the allocator defers the actual 
 * release until that work has completed. Callers announce such use through 
 * @ref record_use; this is the only mechanism the public API exposes for buffer
 * lifetime synchronization. 
 */
class XMIPP4_CORE_API memory_allocator
{
public:
	memory_allocator() noexcept;
	memory_allocator(const memory_allocator &other) = delete;
	memory_allocator(memory_allocator &&other) = delete;
	virtual ~memory_allocator();

	memory_allocator& operator=(const memory_allocator &other) = delete;
	memory_allocator& operator=(memory_allocator &&other) = delete;

	/**
	 * @brief Get the memory resource backing this allocator.
	 *
	 * @return Reference to the backing memory resource.
	 */
	virtual
	const memory_resource& get_memory_resource() const noexcept = 0;

	/**
	 * @brief Get the largest alignment this allocator can satisfy.
	 *
	 * The returned value is the strictest alignment that may be passed to 
	 * @ref allocate. It is a power of two and is constant over the lifetime of
	 * the allocator.
	 *
	 * @return Maximum supported alignment, in bytes.
	 */
	virtual std::size_t get_max_alignment() const noexcept = 0;

	/**
	 * @brief Allocate a new buffer on the backing memory resource.
	 *
	 * The allocator may round @p size up to satisfy internal alignment or page
	 * granularity constraints; the resulting buffer's @ref buffer::get_size 
	 * may return a number greater than or equal to @p size .
	 *
	 * @param size Requested minimum size, in bytes. 
	 * @param alignment Requested alignment, in bytes. Must be a power
	 * of two and not greater than @ref get_max_alignment.
	 * @param queue_hint Optional non-owning pointer to the queue on
	 * which the buffer is expected to be first used. Backends with
	 * per-device or per-queue allocation pools may use it to pick a
	 * cheaper pool. The hint does not establish a use record: callers
	 * still need to invoke @ref record_use before submitting work that
	 * references the buffer. Using the buffer on a queue other than the one 
	 * provided without additional synchronization may lead to undefined 
	 * behavior.
	 * @return The newly allocated buffer. Never null.
	 *
	 * @throws std::bad_alloc (or a backend-specific exception derived
	 * from @c std::exception) on allocation failure.
	 * @throws std::invalid_argument if @p alignment is not a valid
	 * power of two within @ref get_max_alignment.
	 */
	virtual
	std::shared_ptr<buffer> allocate(
		std::size_t size,
		std::size_t alignment,
		device_queue *queue_hint = nullptr
	) = 0;

	/**
	 * @brief Record that a buffer is in use on a queue.
	 *
	 * Notifies the allocator that work referencing @p buffer has been submitted
	 * to @p queue up to the current point in the queue's timeline. When the 
	 * buffer goes out of scope, the allocator postpones the physical release 
	 * until every such recorded use has completed, preventing the underlying 
	 * storage from being reused while device work still depends on it.
	 *
	 * Calls are cumulative: invoking @ref record_use multiple times on the same
	 * buffer, with the same or different queues, extends the set of completions
	 * the allocator must observe before releasing the buffer. Calls referring 
	 * to commands already completed are harmless no-ops.
	 *
	 * @param buffer Buffer being used. Must have been produced by this
	 * allocator; passing a buffer produced by a different allocator
	 * yields undefined behavior.
	 * @param queue Queue on which the buffer is being used.
	 */
	virtual
	void record_use(const buffer &buffer, device_queue &queue) = 0;
};

} // namespace hardware
} // namespace xmipp4
