// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/cpu/cpu_device_queue.hpp>

namespace xmipp4
{
namespace hardware
{

void cpu_device_queue::wait_until_completed() const
{
	// No-op
}

bool cpu_device_queue::is_idle() const noexcept
{
	return true;
}

} // namespace hardware
} // namespace xmipp4
