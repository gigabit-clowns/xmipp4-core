// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_event.hpp>

#include <xmipp4/core/exceptions/invalid_operation_error.hpp>

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

void cpu_event::signal(command_queue &)
{
	// No state to update: cpu_command_queue work is synchronous, so the
	// signal point is reached the moment this call returns.
}

void cpu_event::wait(command_queue &) const
{
	// No-op: cpu_command_queue work is synchronous, so any recorded
	// signal point has already been reached by the time control returns
	// from signal().
}

void cpu_event::wait() const
{
	// No-op for the same reason as wait(command_queue&).
}

bool cpu_event::is_signaled() const
{
	return true;
}

device_timeline_clock::time_point cpu_event::get_timestamp() const
{
	throw invalid_operation_error(
		"cpu_event does not support timestamp queries; "
		"create the event with event_usage_flag_bits::timestamp "
		"to obtain a cpu_timestamped_event instead."
	);
}

} // namespace hardware
} // namespace xmipp4
