// SPDX-License-Identifier: GPL-3.0-only

#include "cpu_event.hpp"

namespace xmipp4
{
namespace hardware
{

event_usage_flags cpu_event::get_supported_usage() const noexcept
{
	return {
		event_usage_flag_bits::host_query,
		event_usage_flag_bits::host_wait,
		event_usage_flag_bits::device_wait
	};
}

void cpu_event::wait() const
{
	// No-op
}

bool cpu_event::is_signaled() const
{
	return true;
}

} // namespace hardware
} // namespace xmipp4
