// SPDX-License-Identifier: GPL-3.0-only

#pragma once

#include <xmipp4/core/hardware/event.hpp>

namespace xmipp4
{

/**
 * @brief CPU implementation of @ref event.
 *
 * Supports @ref event_usage_flag_bits::host_query,
 * @ref event_usage_flag_bits::host_wait and
 * @ref event_usage_flag_bits::device_wait.
 */
class cpu_event
	: public event
{
public:
	event_usage_flags get_supported_usage() const noexcept override;

	void wait() const override;
	bool is_signaled() const override;
};

} // namespace xmipp4
