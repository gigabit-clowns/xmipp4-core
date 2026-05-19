// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/event.hpp>

namespace xmipp4
{
namespace hardware
{

/**
 * @brief CPU implementation of @ref event without timestamp tracking.
 *
 * Work submitted to a @ref cpu_command_queue executes synchronously within
 * the host process, so every signal recorded via
 * @ref command_queue::signal is considered to have been reached at the
 * moment it is recorded. @ref wait calls are non-blocking and
 * @ref is_signaled always returns @c true (the initial state of the event
 * is signaled, as required by the @ref event contract).
 *
 * Supports @ref event_usage_flag_bits::host_query,
 * @ref event_usage_flag_bits::host_wait and
 * @ref event_usage_flag_bits::device_wait. Cross-backend
 * @ref event_usage_flag_bits::cross_device_wait is not supported.
 *
 * Use @ref cpu_timestamped_event when
 * @ref event_usage_flag_bits::timestamp is required.
 */
class cpu_event
	: public event
{
public:
	cpu_event() = default;
	~cpu_event() override = default;

	event_usage_flags get_supported_usage() const noexcept override;

	void wait() const override;
	bool is_signaled() const override;
};

} // namespace hardware
} // namespace xmipp4
