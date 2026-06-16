// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "operation_dispatcher.hpp"
#include "operation_command_manager.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/hardware/device_context.hpp>
#include <xmipp4/core/hardware/command_queue.hpp>

#include <memory>

namespace xmipp4
{
namespace multidimensional
{

/**
 * @brief Bundle of the resources required to execute an operation.
 *
 * An @c execution_context groups the three pieces that @ref execute needs to
 * carry out an operation:
 * - a @ref hardware::device_context, providing the device and its allocators;
 * - a @ref hardware::command_queue (the @em active queue), onto which the work
 *   produced by the operation is submitted;
 * - an @ref operation_dispatcher, encoding the execution strategy (e.g. eager
 *   submission) that turns the operation and its operands into device work.
 *
 * The context is a lightweight, copyable value that merely holds shared
 * references to these resources; copies share the same underlying device,
 * queue and dispatcher. A default-constructed context holds none of them and
 * cannot be used to execute until it is populated.
 *
 * The attributes are mutated through the @ref on_device, @ref on_queue and
 * @ref with_dispatcher methods, each of which returns a modified copy instead
 * of altering the receiver in place. This makes a context safe to share while
 * still allowing callers to derive variants that, for instance, target a
 * different queue of the same device.
 *
 * Selecting a device through @ref on_device (or the device-taking
 * constructors) also sets the active queue to that device's default queue, so
 * that the resulting context is immediately usable. The queue can afterwards
 * be overridden with @ref on_queue.
 */
class execution_context
{
public:
	/**
	 * @brief Construct an empty context.
	 *
	 * The resulting context holds no device, no active queue and no
	 * dispatcher. It must be populated through @ref on_device,
	 * @ref with_dispatcher (and optionally @ref on_queue) before it can be
	 * used to execute an operation.
	 */
	XMIPP4_CORE_API
	execution_context() noexcept;

	/**
	 * @brief Construct a context with an eager dispatcher.
	 *
	 * Builds an @ref eager_operation_dispatcher backed by @p command_manager
	 * and a command cache of the default capacity
	 * (@c XMIPP4_DEFAULT_OPERATION_COMMAND_CACHE_CAPACITY). The active queue is
	 * set to the default queue of @p device_context.
	 *
	 * @param device_context The device on which operations are executed. When
	 * not null, its default queue becomes the active queue.
	 * @param command_manager The manager queried by the dispatcher to build
	 * the command for each operation. Must not be null.
	 *
	 * @throws std::invalid_argument When @p command_manager is null.
	 */
	XMIPP4_CORE_API
	execution_context(
		std::shared_ptr<const hardware::device_context> device_context,
		std::shared_ptr<const operation_command_manager> command_manager
	);

	/**
	 * @brief Construct a context with an explicit dispatcher.
	 *
	 * The active queue is set to the default queue of @p device_context.
	 *
	 * @param device_context The device on which operations are executed. When
	 * not null, its default queue becomes the active queue.
	 * @param dispatcher The dispatcher encoding the execution strategy.
	 */
	XMIPP4_CORE_API
	execution_context(
		std::shared_ptr<const hardware::device_context> device_context,
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
	 * @brief Get the device context on which operations are executed.
	 *
	 * @return Shared pointer to the device context, or null when none is set.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<const hardware::device_context>&
	get_device_context() const noexcept;

	/**
	 * @brief Get the active queue onto which operations are submitted.
	 *
	 * @return Shared pointer to the active command queue, or null when none is
	 * set.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<hardware::command_queue>&
	get_active_queue() const noexcept;

	/**
	 * @brief Get the dispatcher that carries out operations.
	 *
	 * @return Shared pointer to the dispatcher, or null when none is set.
	 */
	XMIPP4_CORE_API
	const std::shared_ptr<multidimensional::operation_dispatcher>&
	get_dispatcher() const noexcept;

	/**
	 * @brief Make a copy of this context targeting a different device.
	 *
	 * The returned context shares this context's dispatcher but uses
	 * @p device_context as its device. Its active queue is set to the default
	 * queue of @p device_context, or to null when @p device_context is null.
	 * This context is left unchanged.
	 *
	 * @param device_context The device to target.
	 * @return A copy of this context with the device (and active queue)
	 * replaced.
	 */
	XMIPP4_CORE_API
	execution_context on_device(
		std::shared_ptr<const hardware::device_context> device_context
	) const;

	/**
	 * @brief Make a copy of this context using a different active queue.
	 *
	 * The returned context shares this context's device and dispatcher but
	 * submits work onto @p queue. This context is left unchanged.
	 *
	 * @param queue The active queue to use. It should belong to this context's
	 * device.
	 * @return A copy of this context with the active queue replaced.
	 */
	XMIPP4_CORE_API
	execution_context on_queue(
		std::shared_ptr<hardware::command_queue> queue
	) const;

	/**
	 * @brief Make a copy of this context using a different memory allocator.
	 *
	 * The returned context shares this context's resources, but replaces the
	 * allocator installed for the provided @p affinity.
	 *
	 * @param affinity The affinity of the allocator to be installed. Must
	 * be valid.
	 * @param allocator The allocator to be installed.
	 * @return A copy of this context with the allocator replaced.
	 */
	XMIPP4_CORE_API
	execution_context with_allocator(
		hardware::memory_resource_affinity affinity,
		std::shared_ptr<hardware::memory_allocator> allocator
	) const;

	/**
	 * @brief Make a copy of this context using a different dispatcher.
	 *
	 * The returned context shares this context's device and active queue but
	 * executes operations through @p dispatcher. This context is left
	 * unchanged.
	 *
	 * @param dispatcher The dispatcher to use.
	 * @return A copy of this context with the dispatcher replaced.
	 */
	XMIPP4_CORE_API
	execution_context with_dispatcher(
		std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
	) const;

private:
	execution_context(
		std::shared_ptr<const hardware::device_context> device_context,
		std::shared_ptr<hardware::command_queue> active_queue,
		std::shared_ptr<multidimensional::operation_dispatcher> dispatcher
	) noexcept;

	std::shared_ptr<const hardware::device_context> m_device_context;
	std::shared_ptr<hardware::command_queue> m_active_queue;
	std::shared_ptr<multidimensional::operation_dispatcher> m_dispatcher;
};

} // namespace multidimensional
} // namespace xmipp4
