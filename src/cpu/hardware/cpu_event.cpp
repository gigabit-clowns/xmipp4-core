// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_event.hpp>

namespace xmipp4
{
namespace hardware
{

event_usage_flags
cpu_event::get_supported_usage() const noexcept
{
	return {
		event_usage_flag_bits::host_query,
		event_usage_flag_bits::host_wait,
		event_usage_flag_bits::device_wait
	};
}

void cpu_event::wait() const
{
	// No-op: cpu_command_queue work is synchronous, so any recorded
	// signal point has already been reached by the time control returns
	// from command_queue::signal.
}

bool cpu_event::is_signaled() const
{
	return true;
}

} // namespace hardware
} // namespace xmipp4
