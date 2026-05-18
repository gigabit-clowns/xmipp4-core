// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_command_queue.hpp>

namespace xmipp4
{
namespace hardware
{

void cpu_command_queue::wait_until_completed() const
{
	// No thread pool wired in yet; the queue holds no in-flight work.
}

bool cpu_command_queue::is_idle() const
{
	return true;
}

} // namespace hardware
} // namespace xmipp4
