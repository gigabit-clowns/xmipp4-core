// SPDX-License-Identifier: GPL-3.0-only

#include <xmipp4/core/compute/cpu/cpu_device_queue.hpp>

namespace xmipp4
{
namespace compute
{

void cpu_device_queue::wait_until_completed() const
{
    // NO-OP
}

bool cpu_device_queue::is_idle() const noexcept
{
    return true;
}

} // namespace compute
} // namespace xmipp4
