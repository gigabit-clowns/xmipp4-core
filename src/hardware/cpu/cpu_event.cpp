// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/hardware/cpu/cpu_event.hpp>


namespace xmipp4
{
namespace hardware
{

void cpu_event::signal(device_queue&)
{
    // No-op
}

void cpu_event::wait() const
{
    // No-op
}

void cpu_event::wait(device_queue&) const
{
    // No-op
}

bool cpu_event::is_signaled() const
{
    return true;
}

} // namespace hardware
} // namespace xmipp4
