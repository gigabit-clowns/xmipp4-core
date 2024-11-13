#pragma once

/***************************************************************************
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307  USA
 *
 *  All comments concerning this program package may be sent to the
 *  e-mail address 'xmipp@cnb.csic.es'
 ***************************************************************************/

/**
 * @file host_to_device_event.hpp
 * @author Oier Lauzirika Zarrabeitia (oierlauzi@bizkaia.eu)
 * @brief Defines the compute::host_to_device_event interface
 * @date 2024-11-13
 * 
 */

#include "../platform/dynamic_shared_object.h"

namespace xmipp4 
{
namespace compute
{

class device_queue;



/**
 * @brief Abstract class describing a device to event.
 * 
 * Instances of this inteface may be used to block the
 * execution of a queue until a signal from the host is received.
 * 
 * @note When constructing a new event, it is constructed in the signaled
 * state, such that waiting on it does not block execution.
 * 
 */
class XMIPP4_CORE_API host_to_device_event
{
public:
    host_to_device_event() = default;
    host_to_device_event(const host_to_device_event &other) = default;
    host_to_device_event(host_to_device_event &&other) = default;
    virtual ~host_to_device_event() = default;

    host_to_device_event& operator=(const host_to_device_event &other) = default;
    host_to_device_event& operator=(host_to_device_event &&other) = default;

    /**
     * @brief Signal the event.
     * 
     */
    virtual void record() = 0;

    /**
     * @brief Wait until the event last recorded event is signaled by the host.
     * 
     * @param queue The queue to be blocked until the event is signaled.
     * 
     */
    virtual void wait(device_queue &queue) = 0;

}; 

} // namespace compute
} // namespace xmipp4
