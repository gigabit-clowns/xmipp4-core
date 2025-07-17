// SPDX-License-Identifier: GPL-3.0-only

/**
 * @file cpu_device_queue.cpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Implementation of cpu_device_queue.hpp
 * @date 2024-10-29
 * 
 */

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
