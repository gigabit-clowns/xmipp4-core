// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include "dispatcher.hpp"

#include <xmipp4/core/platform/dynamic_shared_object.h>
#include <xmipp4/core/hardware/device_context.hpp>

#include <memory>

namespace xmipp4
{
namespace execution
{


/**
 * @brief Pairs the hardware resources and the dispatch strategy used to run
 * operations.
 *
 * An @c context bundles the two pieces of state that every
 * operation needs in order to run: a
 * @ref hardware::device_context describing @em where the work executes (the
 * device, its active @ref hardware::command_queue and its
 * @ref hardware::memory_allocator slots), and a @ref dispatcher
 * describing @em how operations are turned into device work.
 *
 * Like @ref hardware::device_context, the context is a lightweight, copyable
 * value with functional-update semantics: @ref with_device_context and
 * @ref with_dispatcher do not mutate the receiver, they return a modified copy.
 * This makes a context cheap to share (the device context and the dispatcher
 * are both held by shared pointer) while letting callers derive scoped variants,
 * e.g. to run a single operation on a different queue without disturbing any
 * other user of the original context.
 *
 * @par Empty state
 * A default-constructed or moved-from context is @em empty: its device context
 * is empty (see @ref hardware::device_context) and its dispatcher is null.
 * Querying it is well defined; the accessors simply return an empty device
 * context and a null dispatcher.
 */
class context
{
public:
	/**
	 * @brief Construct an empty context.
	 *
	 * The resulting context holds an empty @ref hardware::device_context and a
	 * null dispatcher.
	 */
	XMIPP4_CORE_API
	context() noexcept;

	/**
	 * @brief Construct a context with an explicit dispatcher.
	 *
	 * @param device_context The hardware resources on which operations execute.
	 * @param dispatcher The dispatcher used to turn operations into device 
	 * work.
	 */
	XMIPP4_CORE_API
	context(
		hardware::device_context device_context,
		std::shared_ptr<dispatcher> dispatcher
	);

	XMIPP4_CORE_API
	context(const context &other);
	XMIPP4_CORE_API
	context(context &&other) noexcept;
	XMIPP4_CORE_API
	~context();

	XMIPP4_CORE_API
	context& operator=(const context &other);
	XMIPP4_CORE_API
	context& operator=(context &&other) noexcept;

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
	const std::shared_ptr<dispatcher>&
	get_dispatcher() const noexcept;

	/**
	 * @brief Derive a context with a different device context.
	 *
	 * Replaces the hardware resources describing @em where operations execute
	 * while preserving the dispatcher. Per-use derivations such as switching
	 * queue or overriding an allocator are expressed on the
	 * @ref hardware::device_context itself, e.g.
	 * `ctx.with_device_context(ctx.get_device_context().on_queue(queue));`
	 *
	 * @param device_context The device context of the returned context.
	 * @return A copy of this context with @p device_context as its device
	 * context and the same dispatcher.
	 */
	XMIPP4_CORE_API
	context with_device_context(
		hardware::device_context device_context
	) const;

	/**
	 * @brief Derive a context with a different dispatcher.
	 *
	 * @param dispatcher The dispatcher of the returned context.
	 * @return A copy of this context with @p dispatcher as its dispatcher and
	 * the same device context.
	 */
	XMIPP4_CORE_API
	context with_dispatcher(
		std::shared_ptr<dispatcher> dispatcher
	) const;

private:
	hardware::device_context m_device_context;
	std::shared_ptr<dispatcher> m_dispatcher;
};

} // namespace execution
} // namespace xmipp4
