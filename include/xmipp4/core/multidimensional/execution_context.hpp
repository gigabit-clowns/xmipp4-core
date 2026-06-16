// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_dispatcher.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/hardware/device_context.hpp>

#include <memory>

namespace xmipp4
{
namespace multidimensional
{


/**
 * @brief Pairs the hardware resources and the dispatch strategy used to run
 * operations.
 *
 * An @c execution_context bundles the two pieces of state that every
 * multidimensional operation needs in order to run: a
 * @ref hardware::device_context describing @em where the work executes (the
 * device, its active @ref hardware::command_queue and its
 * @ref hardware::memory_allocator slots), and an @ref operation_dispatcher
 * describing @em how operations are turned into device work.
 *
 * Like @ref hardware::device_context, the context is a lightweight, copyable
 * value with functional-update semantics: @ref on_device, @ref on_queue,
 * @ref with_allocator and @ref with_dispatcher do not mutate the receiver, they
 * return a modified copy. This makes a context cheap to share (the device
 * context and the dispatcher are both held by shared pointer) while letting
 * callers derive scoped variants, e.g. to run a single operation on a different
 * queue without disturbing any other user of the original context.
 *
 * @par Empty state
 * A default-constructed or moved-from context is @em empty: its device context
 * is empty (see @ref hardware::device_context) and its dispatcher is null.
 * Querying it is well defined; the accessors simply return an empty device
 * context and a null dispatcher.
 */
class execution_context
{
public:
	/**
	 * @brief Construct an empty context.
	 *
	 * The resulting context holds an empty @ref hardware::device_context and a
	 * null dispatcher.
	 */
	XMIPP4_CORE_API
	execution_context() noexcept;

	/**
	 * @brief Construct a context with an explicit dispatcher.
	 *
	 * @param device_context The hardware resources on which operations execute.
	 * @param dispatcher The dispatcher used to turn operations into device 
	 * work.
	 */
	XMIPP4_CORE_API
	execution_context(
		hardware::device_context device_context,
		std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
	);

	XMIPP4_CORE_API
	execution_context(const execution_context &other);
	XMIPP4_CORE_API
	execution_context(execution_context &&other) noexcept;
	XMIPP4_CORE_API
	~execution_context();

	XMIPP4_CORE_API
	execution_context& operator=(const execution_context &other);
	XMIPP4_CORE_API
	execution_context& operator=(execution_context &&other) noexcept;

	/**
	 * @brief Retrieve the device context describing where operations execute.
	 *
	 * @return A reference to the wrapped device context.
	 */
	XMIPP4_CORE_API
	const hardware::device_context&
	get_device_context() const noexcept;

	/**
	 * @brief Retrieve the dispatcher used to execute operations.
	 *
	 * @return A reference to the dispatcher, or to a null pointer when this
	 * context is empty.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<multidimensional::operation_dispatcher>&
	get_dispatcher() const noexcept;

	/**
	 * @brief Derive a context bound to a different device.
	 *
	 * Rebinds execution to a fresh @ref hardware::device_context wrapping
	 * @p device_instance while preserving the current dispatcher.
	 *
	 * @param device_instance The device instance to bind. Must not be null.
	 * @return A copy of this context whose device context wraps
	 * @p device_instance.
	 * @throws std::invalid_argument if @p device_instance is null.
	 */
	XMIPP4_CORE_API
	execution_context on_device(
		std::shared_ptr<const hardware::device_instance> device_instance
	) const;

	/**
	 * @brief Derive a context that submits onto a different queue.
	 *
	 * Forwards to @ref hardware::device_context::on_queue while preserving the
	 * dispatcher.
	 *
	 * @param queue The active queue of the returned context. Passing null
	 * reverts to the device's default queue. The queue must belong to the 
	 * device held by this context.
	 * @return A copy of this context with @p queue as its active queue.
	 */
	XMIPP4_CORE_API
	execution_context on_queue(
		std::shared_ptr<hardware::command_queue> queue
	) const;

	/**
	 * @brief Derive a context with a different allocator for an affinity.
	 *
	 * Forwards to @ref hardware::device_context::with_allocator while 
	 * preserving the dispatcher.
	 *
	 * @param affinity The affinity slot to update.
	 * @param allocator The override to install, or null to revert the slot to
	 * the instance allocator for @p affinity.
	 * @return A copy of this context with the updated allocator slot.
	 */
	XMIPP4_CORE_API
	execution_context with_allocator(
		hardware::memory_resource_affinity affinity,
		std::shared_ptr<hardware::memory_allocator> allocator
	) const;

	/**
	 * @brief Derive a context with a different dispatcher.
	 *
	 * @param dispatcher The dispatcher of the returned context.
	 * @return A copy of this context with @p dispatcher as its dispatcher and
	 * the same device context.
	 */
	XMIPP4_CORE_API
	execution_context with_dispatcher(
		std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
	) const;

private:
	hardware::device_context m_device_context;
	std::shared_ptr<multidimensional::operation_dispatcher> m_dispatcher;
};

} // namespace multidimensional
} // namespace xmipp4
