// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_timestamped_event.hpp>

#include <xmipp4/core/platform/constexpr.hpp>

#include <chrono>

namespace xmipp4
{
namespace hardware
{

namespace
{

std::int64_t sample_now_ns() noexcept
{
	using namespace std::chrono;
	return duration_cast<nanoseconds>(
		steady_clock::now().time_since_epoch()
	).count();
}

} // namespace

cpu_timestamped_event::cpu_timestamped_event() noexcept
	: m_last_timestamp_ns(0)
{
}

event_usage_flags
cpu_timestamped_event::get_supported_usage() const noexcept
{
	return {
		event_usage_flag_bits::timestamp,
		event_usage_flag_bits::host_query,
		event_usage_flag_bits::host_wait,
		event_usage_flag_bits::device_wait
	};
}

void cpu_timestamped_event::signal(command_queue &)
{
	m_last_timestamp_ns.store(sample_now_ns(), std::memory_order_release);
}

void cpu_timestamped_event::wait(command_queue &) const
{
	// No-op: cpu_command_queue work is synchronous, so any recorded
	// signal point has already been reached by the time control returns
	// from signal().
}

void cpu_timestamped_event::wait() const
{
	// No-op for the same reason as wait(command_queue&).
}

bool cpu_timestamped_event::is_signaled() const
{
	return true;
}

device_timeline_clock::time_point
cpu_timestamped_event::get_timestamp() const
{
	return device_timeline_clock::time_point(
		device_timeline_clock::duration(
			m_last_timestamp_ns.load(std::memory_order_acquire)
		)
	);
}

} // namespace hardware
} // namespace xmipp4
