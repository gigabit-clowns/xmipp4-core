// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/platform/operating_system.h>

namespace xmipp4
{

/**
 * @brief Arrange for a set of handlers to run around every fork.
 *
 * A forked process keeps the whole address space of its parent but only the
 * thread that asked for the fork. Every other thread is gone, while the memory
 * describing it is still there and still looks alive: a mutex one of them held
 * stays held for good, and a handle naming one of them can never be joined.
 * Anything owning threads therefore has to be told that a fork is coming.
 *
 * @p prepare runs in the parent, before the fork, while every thread is still
 * alive, which is the only moment at which threads can be brought down
 * cleanly. @p parent then runs in the parent and @p child in the child, both
 * after the fork, and both on the one thread that survived it.
 *
 * On a platform without fork this does nothing, there being nothing that could
 * ever call the handlers.
 *
 * @param prepare Called before the fork, in the parent.
 * @param parent Called after the fork, in the parent.
 * @param child Called after the fork, in the child.
 *
 * @note The handlers cannot be removed once registered, so they must not
 * outlive what they act on.
 */
void register_fork_handler(
	void (*prepare)(),
	void (*parent)(),
	void (*child)()
);

} // namespace xmipp4

#if XMIPP4_POSIX
	#include "fork_handler_posix.inl"
#elif XMIPP4_WINDOWS
	#include "fork_handler_windows.inl"
#else
	#error "No fork_handler implementation available for this platform"
#endif
