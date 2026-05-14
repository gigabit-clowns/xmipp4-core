// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_device_event.hpp>

#include <xmipp4/core/platform/constexpr.hpp>

namespace xmipp4
{
namespace hardware
{

device_event_usage_flags
cpu_device_event::get_supported_usage() const noexcept
{
	return {
		device_event_usage_flag_bits::host_query,
		device_event_usage_flag_bits::host_wait,
		device_event_usage_flag_bits::device_wait
	};
}

void cpu_device_event::signal(device_queue &)
{
	// No state to update: cpu_device_queue work is synchronous, so the
	// signal point is reached the moment this call returns.
}

void cpu_device_event::wait(device_queue &) const
{
	// No-op: cpu_device_queue work is synchronous, so any recorded
	// signal point has already been reached by the time control returns
	// from signal().
}

void cpu_device_event::wait() const
{
	// No-op for the same reason as wait(device_queue&).
}

bool cpu_device_event::is_signaled() const
{
	return true;
}

device_timeline_clock::time_point cpu_device_event::get_timestamp() const
{
	// Precondition violation: device_event_usage_flag_bits::timestamp is
	// not advertised by this class, so callers must not invoke this method.
	// Returning a default-constructed time point keeps the override
	// well-formed without claiming a meaningful value.
	return device_timeline_clock::time_point();
}

} // namespace hardware
} // namespace xmipp4
