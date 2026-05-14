// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/cpu/hardware/cpu_device_event.hpp>

#include <xmipp4/core/platform/constexpr.hpp>

#include <chrono>

namespace xmipp4
{
namespace hardware
{

namespace
{

XMIPP4_CONSTEXPR
device_event_usage_flags cpu_supported_usage() noexcept
{
	device_event_usage_flags result;
	result |= device_event_usage_flag_bits::timestamp;
	result |= device_event_usage_flag_bits::host_query;
	result |= device_event_usage_flag_bits::host_wait;
	result |= device_event_usage_flag_bits::device_wait;
	return result;
}

std::int64_t sample_now_ns() noexcept
{
	using namespace std::chrono;
	return duration_cast<nanoseconds>(
		steady_clock::now().time_since_epoch()
	).count();
}

} // namespace

device_event_usage_flags
cpu_device_event::get_supported_usage() const noexcept
{
	return cpu_supported_usage();
}

void cpu_device_event::signal(device_queue &)
{
	m_timestamp_ns.store(sample_now_ns(), std::memory_order_release);
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
	return device_timeline_clock::time_point(
		device_timeline_clock::duration(
			m_timestamp_ns.load(std::memory_order_acquire)
		)
	);
}

} // namespace hardware
} // namespace xmipp4
