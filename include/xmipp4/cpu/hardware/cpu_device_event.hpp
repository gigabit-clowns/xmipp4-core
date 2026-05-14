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
 * @brief CPU implementation of @ref device_event.
 *
 * Work submitted to a @ref cpu_device_queue executes synchronously within
 * the host process, so every signal is considered to have been reached at
 * the moment it is recorded. @ref wait calls are non-blocking and
 * @ref is_signaled always returns @c true (the initial state of the event
 * is signaled, as required by the @ref device_event contract).
 *
 * Supports @ref device_event_usage_flag_bits::timestamp,
 * @ref device_event_usage_flag_bits::host_query,
 * @ref device_event_usage_flag_bits::host_wait and
 * @ref device_event_usage_flag_bits::device_wait. Cross-backend
 * @ref device_event_usage_flag_bits::cross_device_wait is not supported
 */
class cpu_device_event final
	: public device_event
{
public:
	cpu_device_event() = default;
	~cpu_device_event() override = default;

	device_event_usage_flags get_supported_usage() const noexcept override;

	void signal(device_queue &queue) override;
	void wait(device_queue &queue) const override;
	void wait() const override;
	bool is_signaled() const override;
	device_timeline_clock::time_point get_timestamp() const override;

private:
	std::atomic<std::int64_t> m_timestamp_ns;
};

} // namespace hardware
} // namespace xmipp4
