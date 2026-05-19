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
 * Supports @ref event_usage_flag_bits::host_query,
 * @ref event_usage_flag_bits::host_wait and
 * @ref event_usage_flag_bits::device_wait.
 *
 * Use @ref cpu_timestamped_event when
 * @ref event_usage_flag_bits::timestamp is required.
 */
class cpu_event
	: public event
{
public:
	event_usage_flags get_supported_usage() const noexcept override;

	void wait() const override;
	bool is_signaled() const override;
};

} // namespace hardware
} // namespace xmipp4
