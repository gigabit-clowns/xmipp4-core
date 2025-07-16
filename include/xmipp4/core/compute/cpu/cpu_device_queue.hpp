// SPDX-License-Identifier: GPL-3.0-only

#pragma once

/**
 * @file cpu_device_queue.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines cpu_device_queue interface
 * @date 2024-10-29
 * 
 */

#include "../device_queue.hpp"

namespace xmipp4 
{
namespace compute
{

/**
 * @brief Special implementation of the device_queue interface to be 
 * able to send commands to the host.
 * 
 */
class cpu_device_queue final
    : public device_queue
{
public:
    void wait_until_completed() const override;
    bool is_idle() const noexcept override;

}; 

} // namespace compute
} // namespace xmipp4
