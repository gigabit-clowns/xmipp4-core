// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <atomic>
#include <cstdint>

#include <xmipp4/core/hardware/event.hpp>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief CPU implementation of @ref event with timestamp tracking.
 *
 * Behaves like @ref cpu_event, but additionally records the time at which
 * each signal is observed so that @ref cpu_device::elapsed_time can compute
 * intervals between two such events. Pays the cost of one atomic store per
 * recorded signal and one atomic load per timestamp read; prefer
 * @ref cpu_event when @ref event_usage_flag_bits::timestamp is not required.
 *
 * Supports @ref event_usage_flag_bits::timestamp,
 * @ref event_usage_flag_bits::host_query,
 * @ref event_usage_flag_bits::host_wait and
 * @ref event_usage_flag_bits::device_wait.
 */
class cpu_timestamped_event final
	: public event
{
public:
	event_usage_flags get_supported_usage() const noexcept override;

	void wait() const override;
	bool is_signaled() const override;

	/**
	 * @brief Record the current host time as the last signal point.
	 *
	 * Called by @ref cpu_command_queue::signal at the moment the queue
	 * passes the recorded signal point. Not part of the abstract @ref event
	 * interface.
	 */
	void record_timestamp() noexcept;

	/**
	 * @brief Read the last recorded signal time, in nanoseconds since the
	 * @c std::chrono::steady_clock epoch.
	 *
	 * Returns zero if no signal has been recorded yet.
	 */
	std::int64_t get_timestamp_ns() const noexcept;

private:
	std::atomic<std::int64_t> m_last_timestamp_ns;
};

} // namespace hardware
} // namespace xmipp4
