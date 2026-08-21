// SPDX-License-Identifier: GPL-3.0-only

#include "fork_handler.hpp"

namespace xmipp4
{

inline void register_fork_handler(
	void (*/*prepare*/)(),
	void (*/*parent*/)(),
	void (*/*child*/)()
)
{
	// Windows creates a process by building a fresh address space and running
	// an image in it, never by duplicating the caller, so there is no moment
	// at which a copy of this process could inherit threads that no longer
	// exist. The handlers would have nothing to run for.
}

} // namespace xmipp4
