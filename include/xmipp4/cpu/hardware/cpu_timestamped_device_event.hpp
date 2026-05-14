// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <atomic>
#include <cstdint>

#include <xmipp4/core/hardware/device_event.hpp>
#include <xmipp4/core/hardware/device_timeline_clock.hpp>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief CPU implementation of @ref device_event with timestamp tracking.
 *
 * Behaves like @ref cpu_device_event, but additionally records the time at
 * which each signal is observed and exposes it through @ref get_timestamp.
 * Pays the cost of one atomic store per @ref signal and one atomic load per
 * @ref get_timestamp; prefer @ref cpu_device_event when
 * @ref device_event_usage_flag_bits::timestamp is not required.
 *
 * Supports @ref device_event_usage_flag_bits::timestamp,
 * @ref device_event_usage_flag_bits::host_query,
 * @ref device_event_usage_flag_bits::host_wait and
 * @ref device_event_usage_flag_bits::device_wait. Cross-backend
 * @ref device_event_usage_flag_bits::cross_device_wait is not supported.
 */
class cpu_timestamped_device_event final
	: public device_event
{
public:
	cpu_timestamped_device_event() noexcept;
	~cpu_timestamped_device_event() override = default;

	device_event_usage_flags get_supported_usage() const noexcept override;

	void signal(device_queue &queue) override;
	void wait(device_queue &queue) const override;
	void wait() const override;
	bool is_signaled() const override;
	device_timeline_clock::time_point get_timestamp() const override;

private:
	std::atomic<std::int64_t> m_last_timestamp_ns;
};

} // namespace hardware
} // namespace xmipp4
