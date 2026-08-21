// SPDX-License-Identifier: GPL-3.0-only

#include "fork_handler.hpp"

#include <pthread.h>

namespace xmipp4
{

inline void register_fork_handler(
	void (*prepare)(),
	void (*parent)(),
	void (*child)()
)
{
	::pthread_atfork(prepare, parent, child);
}

} // namespace xmipp4
